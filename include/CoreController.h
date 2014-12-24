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
#ifndef CORECONTROLLER_H
#define CORECONTROLLER_H

#include "TesterBase.h"
#include "Protocol.h"
#include "boost/thread/thread.hpp"
#include "Queue.h"

namespace cubeServer
{
    class CoreController
    {
    public:
        CoreController(TesterBase& tester);
        ~CoreController();
        void Start();                                                /* ���������� */
        void Stop();                                                 /* ֹͣ������ */
        void Suspend();                                              /* ��ʱ���� */
        void Join();                                                 /* �ȴ��߳� */
        void Init();                                                 /* ��������ʼ�� */
        int SendApiMessage(apimsg_t *msg);	                         /* ����Api���� */
        apimsg_t ReceiveApiMessage();                                /* ��Queue�ж�ȡһ���� */
        void SetQueueSize(int socketQSize, int uartQSize);           /* ���ô��ڻ�������С */
        void UnitTest_Push();
        void UnitTest_Pop();
    protected:
    private:
        boost::thread mainThread;                 /* ���߳� */
        boost::thread parseThread;                /* ����log�߳� */
        boost::thread msgThread;                  /* ������Ϣ���Ľ����߳� */

        TesterBase *Tester;                       /* ��ǰ���ƵĲ���ƽ̨ */
        Queue *uartQueue;                         /* �첽���ڻ��λ��� */
        int uartQueueSize;                        /* ���ڻ����С */
        Queue *socketQueue;                       /* �첽�׽��ֻ��λ��� */
        int socketQueueSize;                      /* �׽��ֻ��λ����С */

        void MainTask();                          /* ��״̬���߳����� */
        void ParseToolResultTask();               /* �������ߵ�Log��Emit Ok�¼� */
        void RecvMsgRawTask();                    /* ���մ������� */
        void onApiMessage(apimsg_t *message);     /* ��Ϣ����ص� */
        bool msgIsValid();                        /* ��Ϣ����У�� */
        apimsg_t parseApiMessage();               /* �������� */
        void listenForClient();                   /* �ȴ�����Client������ */
    };

}



#endif // CORECONTROLLER_H
