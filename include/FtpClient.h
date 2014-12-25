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

#ifndef FTPCLIENT_H
#define FTPCLIENT_H

#include "curl/curl.h"
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

/* represent a ftp file */
struct FtpFile {
    string filename;          /* Name */
    string path;
    FILE *stream;             /* Stream */
};

class FtpClient
{
    public:
        FtpClient();
        virtual ~FtpClient();

        void SetFtpAddress(const string &url);                             /* Set the ftp server address, Must call before using */
        void SetAuthorityAccount(const string &user, const string &pwr);   /* Set the ftp account */
        void SetRelativePath(const string &path);
        bool FtpDownloadFile(const string &filename);                            /* Download a file from server */
        bool FtpUploadFile(const string &filename);                              /* Upload a file to the server */
        static bool isValidIp(char *ip, int len);
        /* For test */
        void UnitTest_Get();
        void UnitTest_Post();
        void UnitTest_Print();

    private:
        string ftpUrl;            /* Ftp Server Address */
        string userName;          /* Ftp Server Account Username */
        string passWord;          /* Ftp Server Account Password */
        string relativePath;      /* file save into */

        static size_t yaml_fwrite(void *buf, size_t _size, size_t nmemb, void *stream);

};

#endif // FTPCLIENT_H
