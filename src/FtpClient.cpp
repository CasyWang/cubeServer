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

#include "FtpClient.h"


FtpClient::FtpClient()
{
    //ctor
}

FtpClient::~FtpClient()
{
    //dtor
}

/**< Public function */

/** \brief Set the Ftp Server Address
 *
 * \param   url   commonly the ip/dns address of the server
 * \param
 * \return  none
 *
 */
void FtpClient::SetFtpAddress(const string &url) {
    if(FtpClient::isValidIp((char*)url.c_str(), url.size() + 1)) {
        ftpUrl = url;
    }
    else {
        fprintf(stderr, "Ftp Url is invalid.\n");
    }
}

/** \brief  Set the ftp account
 *
 * \param   user  Ftp Server Account Username
 * \param   pwr   Ftp Server Account Password
 * \return  none
 *
 */
void FtpClient::SetAuthorityAccount(const string &user, const string &pwr) {
    this->userName = user;
    this->passWord = pwr;
}

/** \brief  Where to save the downloading file
 *
 * \param   path the relative path
 * \param
 * \return  none
 *
 */
void FtpClient::SetRelativePath(const string &path_yaml, const string &path_fw) {
    this->relativePathYaml = path_yaml;
    this->relativePathFw = path_fw;
}

/** \brief   Download a file from server
 *
 * \param    filename file name, relative path
 * \param    type     file type, indicate where to write
 * \return   true if ok/false if error
 *
 */
bool FtpClient::FtpDownloadFile(const string &filename, file_t type) {

    /* Verify */
    if(filename.length() == 0 || type < BIN_T || type > YAML_T)
        return false;

    bool ret = false;
    CURL *curl;
    CURLcode res;

    struct FtpFile ftpFile;
    ftpFile.filename = filename;
    ftpFile.path = (type == YAML_T) ? this->relativePathYaml : this->relativePathFw;
    ftpFile.stream = NULL;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();                                                    /* Get a curl object */
    if(curl) {
        string fullPath = this->ftpUrl + "/" + filename;
        curl_easy_setopt(curl, CURLOPT_URL, fullPath.c_str());                  /* Set the url */
        curl_easy_setopt(curl, CURLOPT_USERNAME, this->userName.c_str());       /* Set the username */
        curl_easy_setopt(curl, CURLOPT_PASSWORD, this->passWord.c_str());       /* Set the password */
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, FtpClient::yaml_fwrite);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ftpFile);                    /* Pass the struct to callback */
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);                            /* Enable the debug output */

        res = curl_easy_perform(curl);

        /* cleanup for reuse */
        curl_easy_cleanup(curl);
        /* @TODO: Check this res */
        if(CURLE_OK != res) {
            ret = false;
            fprintf(stderr, "curl perform error: %d\n", res);
        }
        else {
            ret = true;
        }

    }

    /* Close the stream if necessary */
    if(NULL != ftpFile.stream) {
        fclose(ftpFile.stream);
    }

    curl_global_cleanup();

    return ret;
}

/** \brief    Get relative path according to file type
 *
 * \param     type   file type
 * \param
 * \return    relative path
 *
 */
string FtpClient::GetRelativePath(file_t type) {
    if(type == YAML_T) {
        return this->relativePathYaml;
    }
    else {
        return this->relativePathFw;
    }
}

/**< Private function */

/** \brief write function for curl ftp
 *
 * \param   buf    Pointer to the array of elements to be written
 * \param   size   Size in bytes of each element to be written.
 * \param   nmemb  Number of elements, each one with a size of size bytes.
 * \param   stream Pointer to a FILE object that specifies an output stream.
 * \return  The total number of elements successfully written
 *
 */
size_t FtpClient::yaml_fwrite(void *buf, size_t _size, size_t nmemb, void *data) {
    try {
        struct FtpFile *out = (struct FtpFile*)data;

        if(out && !out->stream) {
            /* Open file for writing */
            string strFile = out->path + out->filename;
            out->stream = fopen(strFile.c_str(), "wb+");                       /* Read/Write Create */
            if(!out->stream) {
                fprintf(stderr, "failure, can't open file to write.\n");
                return -1;
            }
        }
        return fwrite(buf, _size, nmemb, out->stream);
    }
    catch(exception e) {
        return -1;
    }
}

/** \brief Determine if a ip address is valid
 *
 * \param  ip    ip address
 * \param  len   the length of ip address
 * \return bool  true if valid /false if invalid
 *
 */
bool FtpClient::isValidIp(char *ip, int len) {
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
            if(num > 255 || num < 0)
                return false;
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


/**< UnitTest */

/** \brief For UnitTest
 *
 * \param  none
 * \param  none
 * \return none
 *
 */
void FtpClient::UnitTest_Get() {
    CURLcode ret;
    CURL *curl = curl_easy_init();
    if(!curl) {
        fprintf(stderr, "Fail to create.");
        return;
    }

    /* Let's get baidu because i love baidu */
    ret = curl_easy_setopt(curl, CURLOPT_URL, "http://www.baidu.com");
    if(ret != CURLE_OK) {
        fprintf(stderr, "Fail to get.\n");
        return;
    }

    ret = curl_easy_perform(curl);
    if(ret != 0) {
        fprintf(stderr, "Fail to getting.\n");
        return;
    }
}

void FtpClient::UnitTest_Print() {
    cout << " URL:" << ftpUrl << endl;
    cout << "NAME:" << userName << endl;
    cout << "PASS:" << passWord << endl;
}
