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

#ifndef QUEUE_H
#define QUEUE_H

#include "boost/thread/mutex.hpp"

/* A loop queue implementation */
class Queue
{
    public:
        Queue(int Size);
        virtual ~Queue();
        int AvailableRead();                                    /* Return the Available size */
        int AvailableWrite();                                   /* return the free size */
        int Pop(char *dest, int len);                           /* Pop up len's byte into dest */
        int Push(char *src, int len);                           /* Push len bytes from src into queue */
        int Read(char *dest, int len);                          /* Read */

    private:
        char *QueueBuf;        /* Memory pool for the queue */
        int QueueSize;         /* Queue Size */
        int queueTail;         /* Queue tail */
        int queueFront;        /* Queue front */
        boost::mutex mtx;      /* Boost mutex */
};

#endif // QUEUE_H
