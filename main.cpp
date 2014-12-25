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
const string gRelativePath = "../../yaml/";

void Help();
bool isValidIp(char *ip, int len);
void GetYamlFromServer();

int main(int argc, char **argv)
{
    bool bEnableLog = false;                /* enable stdout log */
    bool bEnableRpc = false;                /* enable rpc */
    char *ftpUrl = NULL;                    /* Ftp server address */

    cubeServer::MainBoardTester tester;
    cubeServer::CoreController controller(tester);
    controller.Init(GetYamlFromServer);

    //GetYamlFromServer();

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

/** \brief   Get all the files with the specified extension
 *
 * \param    root    search path
 * \param    ext     file extension
 * \param    ret     files match the condition
 * \return
 *
 */
void GetSpecifiedFiles(const fs::path &root, const string &ext, vector<fs::path> &ret) {

    /* If path is not exist or not a directory */
    if(!fs::exists(root) || !fs::is_directory(root))
        return;

    fs::recursive_directory_iterator it(root);
    fs::recursive_directory_iterator endit;

    while(it != endit) {
        if(fs::is_regular_file(*it) && it->path().extension() == ext) {
            ret.push_back(it->path().filename());
        }
        ++it;
    }
}

/** \brief    Get configure file from ftp
 *
 * \param     none
 * \param     none
 * \return    none
 *
 */
void GetYamlFromServer() {

    /* A Demo Implementation */
    vector<string> vectBoard;
    vectBoard.push_back("mbed_v1");
    vectBoard.push_back("seeeduino_v3");

    /* Check if already exist */
    fs::path p(gRelativePath);
    vector<fs::path> vectExist;

    GetSpecifiedFiles(p, ".yaml", vectExist);

    int i;
    for(i = 0; i < vectExist.size(); i++) {
        cout << vectExist[i] << endl;
    }

    FtpClient ftpClient;                                /* Make a Instance */

    ftpClient.SetFtpAddress(gFtpUrl);                   /* Set the server address */
    ftpClient.SetAuthorityAccount(gFtpUser, gFtpPwr);   /* Set account */
    ftpClient.SetRelativePath(gRelativePath);           /* Save to which path */

    for(vector<string>::iterator iter = vectBoard.begin(); iter != vectBoard.end(); iter++) {
        string strFile = *iter + ".yaml";
        /* Search exist item */
        fs::path p(strFile);
        if(std::find(vectExist.begin(), vectExist.end(), p) == vectExist.end()){
            /* Can't find, download from server */
            cout << "starting download " << strFile << "..." <<endl;
            if(!ftpClient.FtpDownloadFile(strFile)) {
                fprintf(stderr, "fail to download %s.", strFile.c_str());
            }
        }
    }
}


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
