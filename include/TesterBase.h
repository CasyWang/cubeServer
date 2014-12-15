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

        /* ��Ҫ��������д���麯�� */
        virtual void DoBurnIcBootLoader() {};                  //��¼����BootLoader
        virtual void DoBurnIfceBootLoader() {};                //��¼�ӿ�IC BootLoader
        virtual void DoBurnTestingFw() {};                     //��¼���Թ̼�
        virtual void DoBurnFactoryFw() {};                     //��¼�����̼�
        virtual void HandShake() {};                           //ͬCoreController����

        /* ����Ҫ��д�ĺ��� */

    protected:
    private:
        std::string avrduePath;                                 //avrdue·��
        bool PlatformState;                                     //ƽ̨����״̬
    };

    /*
      ʵ�������̣��ȵ��û���Ĺ��캯����Ȼ����ʵ������Ա������������������Ĺ��캯��
      ���������Ҫ�к���ʵ�嶨��,��Ȼ�麯����vtable�ᱨδ�������
    */


}

#endif // TESTERBASE_H
