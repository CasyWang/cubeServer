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
#include "CoreController.h"
#include "MainBoardTester.h"
#include "FtpClient.h"

void Help();

int main(int argc, char **argv)
{
    bool bEnableLog = false;                /* enable stdout log */
    bool bEnableRpc = false;                /* enable rpc */

    cubeServer::MainBoardTester tester;
    cubeServer::CoreController controller(tester);
    controller.Init();
    FtpClient ftpClient;
    ftpClient.UnitTest_Get();

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
        if(strstr(argv[i], "--with-log") &&
           (len == strlen("--with-log=0"))) {
               bEnableLog = (atoi(argv[i] + len - 1) == 1) ? true : false;
        }
        else if(strstr(argv[i], "--with-rpc") &&
           (len == strlen("--with-rpc=0"))) {
               bEnableRpc = (atoi(argv[i] + len - 1) == 1) ? true : false;
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


void Help() {
    std::cout << "Usage:" << std::endl;
    std::cout << "    --help              to show this help." << std::endl;
    std::cout << "    --get-conf=...      get the config file from ftp server." << std::endl;
    std::cout << "    --start             start the cubeServer controller." << std::endl;
    std::cout << "    --with-rpc=...      whether to use rpc" << std::endl;
    std::cout << "    --platver=...       testing platform version" << std::endl;
    std::cout << "    --enable-log=...    whether to enable log output" << std::endl;
    std::cout << "  eg: cubeServer --enable-log=1 --start" << std::endl;
}
