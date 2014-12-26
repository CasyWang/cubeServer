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

#include <stdlib.h>
#include <iostream>
#include <vector>
#include "string.h"

#include "CoreController.h"
#include "MainBoardTester.h"
#include "FtpClient.h"
#include "boost/filesystem.hpp"

namespace fs = ::boost::filesystem;

#define BOOST_FILESYSTEM_VERSION 3
#define BOOST_FILESYSTEM_NO_DEPRECATED

const string gFtpUrl = "ftp://104.128.82.197";      /* Ftp Server */
const string gYaml = "test1.txt";
const string gFtpUser = "ftpadmin";
const string gFtpPwr = "1234";
const string gRelativePathYaml = "../../yaml/";
const string gRelativePathFw = "../../firmware/";

void Help();
bool isValidIp(char *ip, int len);
void GetFileFromServer(vector<string> &vectBoard, file_t type);

int main(int argc, char **argv)
{
    bool bEnableLog = false;                /* enable stdout log */
    bool bEnableRpc = false;                /* enable rpc */
    char *ftpUrl = NULL;                    /* Ftp server address */

    FtpClient ftpClient;                                  /* Get a Instance */

    ftpClient.SetFtpAddress(gFtpUrl);                     /* Server URL */
    ftpClient.SetAuthorityAccount(gFtpUser, gFtpPwr);     /* Account */
    ftpClient.SetRelativePath(gRelativePathYaml, gRelativePathFw);    /* Where to save those file */

    cubeServer::MainBoardTester tester;
    cubeServer::CoreController controller(tester);
    controller.Init(&ftpClient);

    if(argc < 2) {
        Help();
        return 0;
    }
    int i = 1;
    for(; i < argc; i++) {
        int len = strlen(argv[i]);
        /*
          enable log
          express1 && express2: if express1 is false, express2 will be ignored
        */
        if(strstr(argv[i], "--with-log=") &&
            (len == strlen("--with-log=0"))) {
               bEnableLog = (atoi(argv[i] + len - 1) == 1) ? true : false;
        }
        else if(strstr(argv[i], "--with-rpc=") &&
            (len == strlen("--with-rpc=0"))) {
               bEnableRpc = (atoi(argv[i] + len - 1) == 1) ? true : false;
        }
        else if(strstr(argv[i], "--server=") &&
            (len > strlen("--server="))) {
             ftpUrl = argv[i] + strlen("--server=");
             std::cout << ftpUrl << std::endl;
             if(!FtpClient::isValidIp(ftpUrl, strlen(argv[i]) - strlen("--server="))) {
                 fprintf(stderr, "ftp url is invalid\n");
                 return 0;
             }
        }
        else if(strstr(argv[i], "--start")) {
            controller.Start();
            controller.Join();
        }
        else {
            Help();
        }
    }
}

/**< Private function */

void Help() {
    std::cout << "Usage:" << std::endl;
    std::cout << "    --help              to show this help." << std::endl;
    std::cout << "    --server=...        specified the server address." << std::endl;
    std::cout << "    --start             start the cubeServer controller." << std::endl;
    std::cout << "    --with-rpc=...      whether to use rpc" << std::endl;
    std::cout << "    --platver=...       testing platform version" << std::endl;
    std::cout << "    --enable-log=...    whether to enable log output" << std::endl;
    std::cout << "  eg: cubeServer --enable-log=1 --start" << std::endl;
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
