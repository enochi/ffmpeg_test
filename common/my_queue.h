#ifndef MY_QUEUE_H
#define MY_QUEUE_H
#include <queue>
#include <condition_variable>
#include "my_type.h"
using std::mutex;
using std::lock_guard;
using std::condition_variable;
using std::unique_lock;
template<typename T>
class my_queue
{
public:
    my_queue():_size(100)
    {}
    my_queue(int size):_size(size)
    {
    }
    int put(const int w,const int h,const char*yuv[],int line_size[] )
    {
        return put( yuv_buf(w,h,yuv,line_size));
    }
    int get(T &buf)
    {
        unique_lock<mutex> guard(_mutex);
//        while(_queue.empty())
//        {
//            //printf("queue is empty wait\n");
//            _cond_empty.wait(guard);
//            //printf("queue empty wait out\n");

//        }
        if(_queue.empty())
            return -1;
        buf = _queue.front();
        _queue.pop();
        return 0;
    }
    int put(const T &buf)
    {
        unique_lock<mutex> guard(_mutex);
//        while(_queue.size() == _size)
//        {
//            //printf("queue is full wait\n");
//            _cond_full.wait(guard);
//            //printf("queue full wait out\n");
//        }

        _queue.push(buf);
       // _cond_empty.notify_all();
        return 0;
    }

    mutex _mutex;
    condition_variable _cond_empty;
    condition_variable _cond_full;
    std::queue<T> _queue;
    int _size;

};
#endif // MY_QUEUE_H
