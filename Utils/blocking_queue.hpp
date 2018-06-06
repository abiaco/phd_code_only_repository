
#ifndef ENOBIORECORD_BLOCKING_QUEUE_H
#define ENOBIORECORD_BLOCKING_QUEUE_H


#include <mutex>
#include <condition_variable>
#include <deque>

using namespace std;

template <typename T> class b_queue
{
private:
    mutex              d_mutex;
    condition_variable d_condition;
    deque<T>           d_queue;
    bool stop = false;
public:

    void push(T const& value) {
        {
            unique_lock<mutex> lock(this->d_mutex);
            d_queue.push_front(value);
        }
        this->d_condition.notify_one();
    }
    T pop() {
        unique_lock<mutex> lock(this->d_mutex);

        this->d_condition.wait(lock, [=]{ return (!this->d_queue.empty() || stop) ; });
        T rc(move(this->d_queue.back()));
        this->d_queue.pop_back();
        return rc;
    }

    void setStop(){
        stop = true;
        this->d_condition.notify_one();
    }

    bool isStopped(){
        return stop;
    }
    bool empty(){

        return this->d_queue.empty();
    }

    unsigned long long int size(){
        return this->d_queue.size();
    }
};

#endif