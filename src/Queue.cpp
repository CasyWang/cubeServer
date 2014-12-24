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

#include "Queue.h"
#include "assert.h"


Queue::Queue(int Size) : QueueSize(Size)
{
    //ctor
    QueueBuf = new char[QueueSize];           /* Get the memory pool from heap */
    queueTail = queueFront = 0;
}

Queue::~Queue()
{
    //dtor
    delete QueueBuf;
}

/** Push len bytes into queue buffer from src.
  @param src            the source of data
  @param len            length of the data
  @return int the real length of push
*/
int Queue::Push(char *src, int len) {

    assert(src);

    int length, ret;
    int avail_write = AvailableWrite();
    length = ret = (avail_write < len) ? avail_write : len;
    mtx.lock();
    while(length--) {
        *(QueueBuf + queueTail) = *src++;    /* queueTail is the offset */
        queueTail++;                         /* Copy one, plus one */
        if(queueTail > QueueSize)
            queueTail = 0;
    }
    mtx.unlock();

    return ret;
}

/** Pop len bytes from queue buffer into dest.
  @param dest           the destination of data
  @param len            length of the data
  @return int the real length of push
*/
int Queue::Pop(char *dest, int len) {

    assert(dest);

    int length, ret;
    int avail_read  = AvailableRead();
    length = ret = (avail_read < len) ? avail_read : len;
    mtx.lock();
    while(length--) {
        *dest++ = *(QueueBuf + queueFront);
        queueFront++;
        if(queueFront > QueueSize)
            queueFront = 0;
    }
    mtx.unlock();

    return ret;
}

/** Return the available bytes for read
  @param  none
  @param  none
  @return The available bytes for read
*/
int Queue::AvailableRead() {
    mtx.lock();
    int occupied = queueTail - queueFront;
    mtx.unlock();
    if(occupied < 0) {
        occupied += QueueSize;
    }
    return occupied;
}

/** Return the available bytes for write
  @param  none
  @param  none
  @return The available bytes for write
*/
int Queue::AvailableWrite() {
    return QueueSize - AvailableRead();
}
