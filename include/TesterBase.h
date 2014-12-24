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
#ifndef TESTERBASE_H
#define TESTERBASE_H

#include <string>
#include <iostream>

namespace cubeServer
{
    class TesterBase
    {
    public:
        TesterBase() {};
        ~TesterBase() {};

        /* 需要派生类重写的虚函数 */
        virtual void DoBurnIcBootLoader() {};                  /* 烧录单板BootLoader */
        virtual void DoBurnIfceBootLoader() {};                /* 烧录接口IC BootLoader */
        virtual void DoBurnTestingFw() {};                     /* 烧录测试固件 */
        virtual void DoBurnFactoryFw() {};                     /* 烧录出厂固件 */

        virtual void ReverseBoardElecLevel() = 0;               /* 翻转待测单板电平 */
        virtual void TestIo() = 0;                              /* 测试单板IO */
        virtual void TestUart() = 0;                            /* 测试单板UART */

        virtual void HandShake() = 0;                           /* 同CoreController握手 */
        virtual void LoadYaml() = 0;                            /* 加载yaml配置文件 */

        /* 不需要重写的函数 */
        //virtual void SetController(CoreController *c) {};       /* 设置本测试平台的控制器句柄 */


    protected:
    private:
        std::string avrduePath;                                 /* avrdue路径 */

        volatile static bool PlatformState;                     /* 平台运行状态 */
    };

    /*
      实例化过程，先调用基类的构造函数，然后是实例化成员变量，最后才是派生类的构造函数
      基类里必须要有函数实体定义,不然虚函数表vtable会报未定义错误
    */


}

#endif // TESTERBASE_H
