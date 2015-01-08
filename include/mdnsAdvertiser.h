/*
The MIT License (MIT)

Copyright (c) <2014> <oliver-lxtech2013@gmail.com>
Thanks to Darell Tan

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
#ifndef MDNSADVERTISER_H
#define MDNSADVERTISER_H

/* A C++ Implementation of mDNS advertiser */

#include <iostream>
#include <string>
#include "boost/thread/mutex.hpp"
#include <boost/asio.hpp>
#include <boost/bind.hpp>

using boost::asio::ip::udp;
using boost::asio::ip::tcp;

#include "mdns.h"
#include <assert.h>


/*
 * IANA assigned IPv4 multicast group for mdns
 */
#define MDNS_IPV4_MULTICAST_GROUP "224.0.0.251"

/*
 * IANA assigned IPv6 multicast group for mdns
 */
#define MDNS_IPV6_MULTICAST_GROUP "FF02::FB"

/*
 * IANA assigned UDP multicast port for mdns
 */
#define MDNS_PORT    5353

#define PACKET_SIZE  65536
#define SERVICES_DNS_SD_NLABEL \
		((uint8_t *) "\x09_services\x07_dns-sd\x04_udp\x05local")

/**< mDNS daemon struct */
struct mdnsd {

    int stop_flag;

    struct rr_group *group;
    struct rr_list *announce;     /* Services which i can provide */
    struct rr_list *services;     /* Services which i received */
    uint8_t *hostname;            /* host name of this device */
    boost::mutex mtx;             /* Boost mutex */
};

/**< mDNS Service */
struct mdns_service {
    struct rr_list *entries;
};

namespace mdns
{

class mdnsAdvertiser
{
    public:
        mdnsAdvertiser(boost::asio::io_service& io_service) : mCastSocket(io_service)
        {
            /* Create the socket so that multiple may be bound to the same address */
            multicast_endpoint = udp::endpoint(
                boost::asio::ip::address_v4::from_string(MDNS_IPV4_MULTICAST_GROUP),
                MDNS_PORT);

            udp::endpoint listen_endpoint(
                boost::asio::ip::address_v4::from_string("0.0.0.0"),         /* Should check what's the difference between 127.0.0.1 and 0.0.0.0 */
                MDNS_PORT);    /* local ip */

            mCastSocket.open(listen_endpoint.protocol());                    /* localhost:MDNS_PORT */
            mCastSocket.set_option(udp::socket::reuse_address(true));

            #ifdef ENABLE_SOCKET_DEBUG
            boost::asio::socket_base::debug option(true);
            mCastSocket.set_option(option);
            #endif

            mCastSocket.bind(listen_endpoint);

            /* Join the multicast group */
            mCastSocket.set_option(boost::asio::ip::multicast::join_group(
                boost::asio::ip::address::from_string(MDNS_IPV4_MULTICAST_GROUP)));

            if(!mCastSocket.is_open()) {
                std::cerr << "socket open failed." << std::endl;
                delete this;                                        /* if socket create failed, delete */
            } else {
                std::string strAddr = multicast_endpoint.address().to_string();
                int Port = multicast_endpoint.port();
                std::cout << "addr:" << strAddr << std::endl;
                std::cout << "port:" << Port << std::endl;
            }
            memset(recvBuf, 0, max_length);
            /* Set async receive */
            /* @TODO: 将remote_endpoint同数据报文关联起来,好从服务中判断 */
            mCastSocket.async_receive_from(
                boost::asio::buffer(recvBuf, max_length), remote_endpoint,
                boost::bind(&mdnsAdvertiser::handleReceive, this,              //不是静态函数,也可以直接通过类名访问?
                            boost::asio::placeholders::error,
                            boost::asio::placeholders::bytes_transferred));

            /* Set async send */
            memset(sendBuf, 0, max_length);
            sprintf(sendBuf, "cube-server\n");
            mCastSocket.async_send_to(
                boost::asio::buffer(sendBuf), multicast_endpoint,
                boost::bind(&mdnsAdvertiser::handleSend, this,
                boost::asio::placeholders::error));

            /*
              The callback will be executed only when io_service.run() is called
              io_service.run() is a async message distribution loop, which works in blocking mode
            */

        }

        virtual ~mdnsAdvertiser() {
            /* Must close the socket when we leave */
            if(mCastSocket.is_open()) {
                mCastSocket.close();
            }
        }

        void StartService() {}

        /** \brief
         *
         * \param
         * \param
         * \return
         *
         */
        void RegisterService(const char *instance_name, const char *type,
                             uint16_t port, const char *txt[]) {

            /**< TODO: Using testPlatform ID to sign this hostname */
            char *hostname = "cube-server.local";
            /* Get local ip */
            std::string strLocalIp = this->mCastSocket.local_endpoint().address().to_string();
            mdnsSetHostName(hostname, inet_addr(strLocalIp.c_str()));

            struct rr_entry *txt_e = NULL,
					*srv_e = NULL,
					*ptr_e = NULL,
					*bptr_e = NULL;

            uint8_t *target;
            uint8_t *inst_nlabel, *type_nlabel, *nlabel;
            struct mdns_service *service = (struct mdns_service *)malloc(sizeof(struct mdns_service));
            memset(service, 0, sizeof(struct mdns_service));

            // combine service name
            type_nlabel = create_nlabel(type);
            inst_nlabel = create_nlabel(instance_name);
            nlabel = join_nlabel(inst_nlabel, type_nlabel);

            // create TXT record
            if (txt && *txt) {
                txt_e = rr_create(dup_nlabel(nlabel), RR_TXT);
                rr_list_append(&service->entries, txt_e);

                // add TXTs
                for (; *txt; txt++)
                    rr_add_txt(txt_e, *txt);
            }

            // create SRV record
            target = dup_nlabel(this->svr->hostname);

            srv_e = rr_create_srv(dup_nlabel(nlabel), port, target);
            rr_list_append(&service->entries, srv_e);

            // create PTR record for type
            ptr_e = rr_create_ptr(type_nlabel, srv_e);

            // create services PTR record for type
            // this enables the type to show up as a "service"
            bptr_e = rr_create_ptr(dup_nlabel(SERVICES_DNS_SD_NLABEL), ptr_e);

            // modify lists here
            this->svr->mtx.lock();

            if (txt_e) {
                rr_group_add(&svr->group, txt_e);
            }

            rr_group_add(&svr->group, srv_e);
            rr_group_add(&svr->group, ptr_e);
            rr_group_add(&svr->group, bptr_e);

            // append PTR entry to announce list
            rr_list_append(&svr->announce, ptr_e);
            rr_list_append(&svr->services, ptr_e);

            this->svr->mtx.unlock();

            // don't free type_nlabel - it's with the PTR record
            free(nlabel);
            free(inst_nlabel);

            // notify server
        }



        int GetService() {}                    /* Get Service from network */


        /** \brief  Publish what i can provide
         *
         * \param
         * \param
         * \return
         *
         */
        int PublishService() {

        }

        /** \brief
         *
         * \param
         * \param
         * \return
         *
         */
        void mdnsSetHostName(const char *hostname, uint32_t ip) {
             struct rr_entry *a_e = NULL;
             struct rr_entry *nsec_e = NULL;

             assert(this->svr->hostname == NULL);

             a_e = rr_create_a(create_nlabel(hostname), ip);
             nsec_e = rr_create_a(create_nlabel(hostname), RR_NSEC);

             rr_set_nsec(nsec_e, RR_A);

             /* lock before we handle this */
             this->svr->mtx.lock();
             this->svr->hostname = create_nlabel(hostname);
             rr_group_add(&this->svr->group, a_e);
             rr_group_add(&this->svr->group, nsec_e);
             this->svr->mtx.unlock();
         }

    private:
        enum { max_length = 1024 };
        uint8_t recvBuf[max_length];                /* 犯了重复造轮子错误 @TODO: 将buffer改为stream */
        char sendBuf[max_length];

        udp::endpoint multicast_endpoint;        /* mDNS multicast endpoint, every client can receive */
        udp::endpoint remote_endpoint;           /* C# controller Client's endpoint */
        udp::socket mCastSocket;                 /* mCastSocket */

        struct mdnsd *svr;                       /* mDNS daemon */
        struct mdns_service *service;            /* mDNS service */


        /** \brief
         *
         * \param
         * \param
         * \return
         *
         */
        void handleReceive(const boost::system::error_code& err, size_t bytes_recvd) {
            /* 在接收到数据后执行此回调 */
            if(!err && bytes_recvd > 0) {
                struct mdns_pkt *replyPkt = new struct mdns_pkt;

                uint8_t *copyBuf = new uint8_t[bytes_recvd];
                memset(copyBuf, 0, bytes_recvd);
                /* @TODO: using mutex to protect critical section */
                memcpy(copyBuf, recvBuf, bytes_recvd);
                struct mdns_pkt *mdnsPkt = mdns_parse_pkt(copyBuf, bytes_recvd);
                if(mdnsPkt != NULL) {
                    /* print id and flag */
                    std::cout << "id:" << mdnsPkt->id << std::endl;
                    std::cout << "flag:" << mdnsPkt->flags << std::endl;
                    std::cout << "num_ans_rr:" << mdnsPkt->num_ans_rr << std::endl;

                    /* process packet */
                    mdnsAdvertiser::processMdnsPkt(this->svr, mdnsPkt, replyPkt);
                }
                //recvBuf[bytes_recvd] = '\0';
                //std::cout << "revd:" << recvBuf << std::endl;
                memset(recvBuf, 0, max_length);
                delete copyBuf;
                mCastSocket.async_receive_from(
                    boost::asio::buffer(recvBuf, max_length), remote_endpoint,
                    boost::bind(&mdnsAdvertiser::handleReceive, this,              //不是静态函数,也可以直接通过类名访问?
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
            }
        }

        /** \brief
         *
         * \param
         * \param
         * \return
         *
         */
        void handleSend(const boost::system::error_code& err) {
            /* 在发送完后才执行此回调 */
            if(!err) {
                std::cout << "send:" << sendBuf << std::endl;

            } else {
                std::cerr << "socket aysnc_send failed." << std::endl;
            }
        }

        /** \brief
         *
         * \param
         * \param
         * \return
         *
         */
        static void processMdnsPkt(struct mdnsd* svr, struct mdns_pkt *pkt, struct mdns_pkt *reply) {
            assert(pkt != NULL);
            /* Check if it's a standard query */
            if((pkt->flags & MDNS_FLAG_RESP) == 0 &&
                MDNS_FLAG_GET_OPCODE(pkt->flags) == 0) {
                //mdns_init_reply(reply, pkt->id);              /* Init reply packet */

                /* Loop through questions */
                struct rr_list *qnl = pkt->rr_qn;
                int i = 0;
                for(; i < pkt->num_qn; i++, qnl = qnl->next) {
                    struct rr_entry *qn = qnl->e;
                    int num_ans_added = 0;

                    char *namestr = nlabel_to_str(qn->name);
                    DEBUG_PRINTF("qn #%d: type %s (%02x) %s - ", i, rr_get_type_name(qn->type), qn->type, namestr);
                    free(namestr);
                    /**< 逻辑: 固定IP连接不上?
                    1.Client启动mDNS查找cubeServer IP
                    2.查找到,则用此IP连接服务.
                    */
                }
            }
        }


         /** \brief
          *
          * \param
          * \param
          * \return
          *
          */
         void mdnsAddResouceRecord(struct rr_entry *rr) {
             this->svr->mtx.lock();
             rr_group_add(&this->svr->group, rr);
             this->svr->mtx.unlock();
         }



};

} //NAME-SPACE



#endif // MDNSADVERTISER_H
