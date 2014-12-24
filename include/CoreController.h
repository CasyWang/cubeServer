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
        void Start();                                                /* 启动控制器 */
        void Stop();                                                 /* 停止控制器 */
        void Suspend();                                              /* 暂时挂起 */
        void Join();                                                 /* 等待线程 */
        void Init();                                                 /* 控制器初始化 */
        int SendApiMessage(apimsg_t *msg);	                         /* 发送Api报文 */
        apimsg_t ReceiveApiMessage();                                /* 从Queue中读取一个包 */
        void SetQueueSize(int socketQSize, int uartQSize);           /* 设置串口缓冲区大小 */
        void UnitTest_Push();
        void UnitTest_Pop();
    protected:
    private:
        boost::thread mainThread;                 /* 主线程 */
        boost::thread parseThread;                /* 解析log线程 */
        boost::thread msgThread;                  /* 串口消息报文接收线程 */

        TesterBase *Tester;                       /* 当前控制的测试平台 */
        Queue *uartQueue;                         /* 异步串口环形缓冲 */
        int uartQueueSize;                        /* 串口缓冲大小 */
        Queue *socketQueue;                       /* 异步套接字环形缓冲 */
        int socketQueueSize;                      /* 套接字环形缓冲大小 */

        void MainTask();                          /* 主状态机线程任务 */
        void ParseToolResultTask();               /* 解析工具的Log，Emit Ok事件 */
        void RecvMsgRawTask();                    /* 接收串口数据 */
        void onApiMessage(apimsg_t *message);     /* 消息到达回调 */
        bool msgIsValid();                        /* 消息报文校验 */
        apimsg_t parseApiMessage();               /* 解析报文 */
        void listenForClient();                   /* 等待控制Client端连接 */
    };

}



#endif // CORECONTROLLER_H
