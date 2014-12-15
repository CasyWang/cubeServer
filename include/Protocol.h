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

//@TODO 采用google protobuffer协议获取JsonRpc实现跨平台控制

/* 控制器状态 */
typedef enum {
	IDLE = 3,                  /* 空闲状态 */
	WAIT_CONNECT,              /* 等待串口连接 */
	CONNECT_ESTABLISH,         /* 连接建立 */
	WAIT_BURN_RESULT,          /* Burn过程，等待Burn结果 */
	BURN_OK,                   /* 收到Burn结果 */
	WAIT_IO_TEST_RESULT,       /* 等待IO测试结果 */
	IO_TEST_OK,                /* IO测试OK */
	TIME_OUT                   /* 超时 */
}state_t;

/* 控制器与主控板之间的报文格式 */
typedef struct {

}apimsg_t;



#endif // PROTOCOL_H_INCLUDED
