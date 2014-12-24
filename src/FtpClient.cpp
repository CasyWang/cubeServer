#include "FtpClient.h"

FtpClient::FtpClient()
{
    //ctor
}

FtpClient::~FtpClient()
{
    //dtor
}

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
