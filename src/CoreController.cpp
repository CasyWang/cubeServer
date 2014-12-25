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
#include "CoreController.h"


namespace cubeServer
{
    CoreController::CoreController(TesterBase& tester)
    : Tester(&tester), uartQueueSize(512), socketQueueSize(512)
    {
        //ctor
    }

    CoreController::~CoreController()
    {
        //dtor
    }

    /* ======================= Public: ======================== */

    /* Start the CoreController */
    void CoreController::Start() {
        /* @TODO:CoreController析构之后,线程不会终止,需要处理 */

        mainThread = boost::thread(&CoreController::MainTask, this);
        parseThread = boost::thread(&CoreController::ParseToolResultTask, this);
        msgThread = boost::thread(&CoreController::RecvMsgRawTask, this);

    }

    /* Stop CoreController */
    void CoreController::Stop() {

    }

    /* main thread wait */
    void CoreController::Join() {
        mainThread.join();            /* main thread wait child thread */
        parseThread.join();
        msgThread.join();
    }

    /* Must Initialize before using */
    void CoreController::Init(PrepareFile func) {
        uartQueue = new Queue(uartQueueSize);               /* 在堆上给Queue分配内存 */
        socketQueue = new Queue(socketQueueSize);
        this->GetYaml = func;
    }


    /* ======================= Private: ======================== */

    /* Core Controller's main state machine thread */
    void CoreController::MainTask() {
        /* Get configure file at the very beginning */
        this->GetYaml();          /* 注意文件root路径 */
        while(1) {
            //Tester->HandShake();
            std::cout << "main task" << std::endl;
            boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
        }
    }

    /* this thread is used to parse the result generated by avrdue */
    void CoreController::ParseToolResultTask() {
        while(1) {
            //std::cout << "CoreController is parsing the avrdue log." << std::endl;
            boost::this_thread::sleep(boost::posix_time::milliseconds(200));
        }
    }

    void CoreController::RecvMsgRawTask() {
        while(1) {

            boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
        }
    }

    /* callback for incoming data */
    void CoreController::onApiMessage(apimsg_t *msg) {

    }

    apimsg_t CoreController::parseApiMessage() {

    }

    int CoreController::SendApiMessage(apimsg_t *msg) {

    }

    apimsg_t CoreController::ReceiveApiMessage() {

    }


    /**< UnitTest */
        void CoreController::UnitTest_Push() {
        int i;
        for(i = 0; i < 600; i++) {
            char random = 1 + rand() % 100;
            uartQueue->Push(&random, sizeof(char));
            std::cout << "push" << i << ":" << (int)random << std::endl;
            std::cout << "pw" << i << ":" << uartQueue->AvailableWrite() << std::endl;
        }
    }

    void CoreController::UnitTest_Pop() {
        int i;
        for(i = 0; i < 600; i++) {
            char buf;
            uartQueue->Pop(&buf, sizeof(char));
            std::cout << "pop" << i << ":" << (int)buf << std::endl;
            std::cout << "pr" << i << ":" << uartQueue->AvailableRead() << std::endl;
        }
    }

} //Namespace


