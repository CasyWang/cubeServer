/*
The MIT License (MIT)

Copyright (c) <2014> <oliver-lxtech2013@gmail.com>

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

#define _WIN32_WINNT 0x501

#include <stdlib.h>
#include <iostream>
#include <vector>
#include "string.h"

/* For mDNS */
#include "mdnsAdvertiser.h"



#include "CoreController.h"
#include "MainBoardTester.h"
#include "FtpClient.h"
#include "boost/program_options.hpp"

namespace po = ::boost::program_options;


string FtpUrl = "";                                 /* Waiting user input */
string FtpUser = "";
string FtpPwr = "";
int ServerPort = 0;                                 /* Socket port server listening */
bool gWithSSL = false;

const string gFtpUrl = "ftp://104.128.82.197";      /* Ftp Server */
const string gYaml = "test1.txt";
const string gFtpUser = "ftpadmin";
const string gFtpPwr = "1234";
const string gRelativePathYaml = "../../yaml/";
const string gRelativePathFw = "../../firmware/";

void Help();
bool isValidIp(char *ip, int len);
static bool ParseCmdLine(int argc, char **argv);

int main(int argc, char **argv)
{
    if(!ParseCmdLine(argc, argv)) {
        return 1;
    }

    try {

        boost::asio::io_service io_service;
        mdns::mdnsAdvertiser advertiser(io_service);
        io_service.run();

        FtpClient ftpClient;                                  /* Get a Instance */

        ftpClient.SetFtpAddress(FtpUrl);                      /* Server URL */
        ftpClient.SetAuthorityAccount(FtpUser, FtpPwr);       /* Account */
        ftpClient.SetRelativePath(gRelativePathYaml, gRelativePathFw);    /* Where to save those file */

        cubeServer::MainBoardTester tester;
        cubeServer::CoreController controller(tester);
        controller.Init(&ftpClient);

        controller.Start();                 /* Start controller */
        controller.Join();                  /* Main thread waiting children thread */
    }
    catch(exception& e) {
        std::cerr << "err: " << e.what() << std::endl;
    }

    return 0;
}

/**< Private function */


static bool ParseCmdLine(int argc, char **argv) {
    try {
        po::options_description desc("Allowed options");
        desc.add_options()
            ("help", "produce this help message.")
            ("with-ssl", po::value<int>(), "enable ssl security.")
            ("ftp-url", po::value<string>(&FtpUrl)->default_value(gFtpUrl), "set the ftp address.")
            ("ftp-user", po::value<string>(&FtpUser)->default_value(gFtpUser), "set the ftp username.")
            ("ftp-password", po::value<string>(&FtpPwr)->default_value(gFtpPwr), "set the ftp password.")
            ("port", po::value<int>(&ServerPort)->default_value(8080), "server listening port.")
            ("with-log", po::value<int>(), "enable log output.");

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        /*
        the vm can be used just like std::map, except that the values
        store there must be retrieved with the 'as' method
        */
        if(vm.count("help")) {
            std::cout << desc << std::endl;
            exit(0);                               /* Any time we call help, will not start program */
        }

        if(vm.count("with-ssl")) {
            std::cout << "with-ssl set to " << vm["with-ssl"].as<int>() << std::endl;
        }

        if(vm.count("ftp-url")) {
            std::cout << "ftp-url set to " << vm["ftp-url"].as<string>() << std::endl;
        }

        if(vm.count("port")) {
            std::cout << "port set to " << vm["port"].as<int>() << std::endl;
        }
    }
    catch (exception& e) {
        std::cerr << "error: " << e.what() << std::endl;
        return false;
    }

    return true;
}


/** \brief Determine if a ip address is valid
 *
 * \param  ip    ip address
 * \param  len   the length of ip address
 * \return bool  true if valid /false if invalid
 *
 */
bool isValidIp(char *ip, int len) {
    /* strtok 必须在heap上操作,已初始数据段、BSS段、函数的参数栈区操作都会报错 */
    try {
        char *tmp = new char[len];        /* 在堆上分配空间 */
        strcpy(tmp, ip);
        int i = 0;
        char *token[4] = { NULL };
        char *p = strtok(tmp, ".");
        while(p != NULL) {
            token[i] = p;
            p = strtok(NULL, ".");
            i++;
        }

        /* must have 4 token */
        if(i < 3) return false;

        /* every token should be number and smaller than 255 */
        for(i = 0; i < 4; i++) {
            int num = atoi(token[i]);        /* @TODO: what if token is a string, not number */
            //std::cout << num << std::endl;
            if(num > 255 || num < 0) return false;
        }

        delete tmp;                   /* Free */
        tmp = NULL;

        return true;
    }
    catch (exception e) {
        fprintf(stderr, "Func 'isValidIp' Err: %s", e.what());
        return false;
    }
}
