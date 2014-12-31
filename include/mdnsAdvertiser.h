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



namespace mdns
{

class mdnsAdvertiser
{
    public:
        mdnsAdvertiser(boost::asio::io_service& io_service) : mCastSocket(io_service, udp::endpoint(udp::v4(), MDNS_PORT))
        {
            /* Create the socket so that multiple may be bound to the same address */
            multicast_endpoint = udp::endpoint(
                boost::asio::ip::address_v4::from_string(MDNS_IPV4_MULTICAST_GROUP),
                5353);

            udp::endpoint listen_endpoint(
                boost::asio::ip::address_v4::from_string("127.0.0.1"),      /* Should check if 127.0.0.1 */
                MDNS_PORT);    /* local ip */

            //mCastSocket.open(listen_endpoint.protocol());                    /* localhost:MDNS_PORT */
            mCastSocket.set_option(udp::socket::reuse_address(true));
            //boost::asio::socket_base::debug option(true);
            //mCastSocket.set_option(option);
            //mCastSocket.bind(listen_endpoint);

            /* Join the multicast group */
            mCastSocket.set_option(boost::asio::ip::multicast::join_group(
                boost::asio::ip::address::from_string(MDNS_IPV4_MULTICAST_GROUP)));

            if(!mCastSocket.is_open()) {
                std::cerr << "socket open failed." << std::endl;
            } else {
                std::string strAddr = multicast_endpoint.address().to_string();
                int Port = multicast_endpoint.port();
                std::cout << "addr:" << strAddr << std::endl;
                std::cout << "port:" << Port << std::endl;
            }
            /* Set async receive */
            /* @TODO: 将remote_endpoint同数据报文关联起来,好从服务中判断 */
            mCastSocket.async_receive_from(
                boost::asio::buffer(recvBuf, max_length), remote_endpoint,
                boost::bind(&mdnsAdvertiser::handleReceive, this,              //不是静态函数,也可以直接通过类名访问?
                            boost::asio::placeholders::error,
                            boost::asio::placeholders::bytes_transferred));



            /* Set async send */
            mCastSocket.async_send_to(
                boost::asio::buffer(sendBuf), multicast_endpoint,
                boost::bind(&mdnsAdvertiser::handleSend, this,
                boost::asio::placeholders::error));

            /*
              The callback will be executed only when io_service.run() is called
              io_service.run() is a async message distribution loop, blocking
            */
        }

        virtual ~mdnsAdvertiser() {
            /* Must close the socket when we leave */
            if(mCastSocket.is_open()) {
                mCastSocket.close();
            }
        }

        void StartService() {}
        void RegisterService() {}              /* Register service */
        int GetService() {}                    /* Get Service from network */

    private:
        enum { max_length = 1024 };
        char recvBuf[max_length];
        char sendBuf[max_length];

        udp::endpoint multicast_endpoint;        /* mDNS multicast endpoint, every client can receive */
        udp::endpoint remote_endpoint;           /* C# controller Client's endpoint */
        udp::socket mCastSocket;                 /* mCastSocket */

        struct mdnsd *svr;                       /* mDNS daemon */

        void handleReceive(const boost::system::error_code& err, size_t bytes_recvd) {
            /* 在接收到数据后执行此回调 */
            if(!err) {
                std::cout << "revd:" << recvBuf << std::endl;
                mCastSocket.async_receive_from(
                    boost::asio::buffer(recvBuf, max_length), remote_endpoint,
                    boost::bind(&mdnsAdvertiser::handleReceive, this,              //不是静态函数,也可以直接通过类名访问?
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
            }
        }

        void handleSend(const boost::system::error_code& err) {
            /* 在发送完后才执行此回调 */
            if(!err) {
                std::cout << "send:" << sendBuf << std::endl;

            } else {
                std::cerr << "socket aysnc_send failed." << std::endl;
            }
        }
};

} //NAME-SPACE



#endif // MDNSADVERTISER_H
