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
#ifndef MAINBOARDTESTER_H
#define MAINBOARDTESTER_H

#include "TesterBase.h"
#include "Protocol.h"
#include <iostream>

namespace cubeServer
{
    typedef int (*callback_send)(apimsg_t *msg);
    typedef apimsg_t (*callback_receive)(void);

    class MainBoardTester : public TesterBase
    {
    public:
        MainBoardTester();
        virtual ~MainBoardTester();

        /* 重写基类方法 */
        void HandShake()
        {
            std::cout << "hand shake--->" << std::endl;
        }

        void Init(callback_send s, callback_receive r) {
            send = s;
            receive = r;
        }

        void ReverseBoardElecLevel() {
            /* 触发中断,翻转待测单板的电平 */
            apimsg_t msg;
            send(&msg);
        }

        void TestIo() {
            apimsg_t msg;
            send(&msg);
        }

        void TestUart() {

        }

        void LoadYaml() {

        }

    protected:
    private:
        callback_send send;            /* 传入的callback,向主控发送报文 */
        callback_receive receive;      /* 穿入的callback,接收主控报文 */
    };
}

#endif // MAINBOARDTESTER_H
