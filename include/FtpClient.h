#ifndef FTPCLIENT_H
#define FTPCLIENT_H

#include "curl/curl.h"


class FtpClient
{
    public:
        FtpClient();
        virtual ~FtpClient();
        void UnitTest_Get();
        void UnitTest_Post();
    protected:
    private:
};

#endif // FTPCLIENT_H
