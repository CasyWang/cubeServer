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

#ifndef PROTOCOL_H_INCLUDED
#define PROTOCOL_H_INCLUDED

//@TODO ����google protobufferЭ���ȡJsonRpcʵ�ֿ�ƽ̨����

/* ������״̬ */
typedef enum {
	IDLE = 3,                  /* ����״̬ */
	WAIT_CONNECT,              /* �ȴ��������� */
	CONNECT_ESTABLISH,         /* ���ӽ��� */
	WAIT_BURN_RESULT,          /* Burn���̣��ȴ�Burn��� */
	BURN_OK,                   /* �յ�Burn��� */
	WAIT_IO_TEST_RESULT,       /* �ȴ�IO���Խ�� */
	IO_TEST_OK,                /* IO����OK */
	TIME_OUT                   /* ��ʱ */
}state_t;

/* �����������ذ�֮��ı��ĸ�ʽ */
typedef struct {

}apimsg_t;



#endif // PROTOCOL_H_INCLUDED
