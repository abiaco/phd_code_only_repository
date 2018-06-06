
#ifndef ENOBIORECORD_LOCK_STATE_H
#define ENOBIORECORD_LOCK_STATE_H

#include <mutex>
#include <condition_variable>
#include <deque>

using namespace std;

class LockState
{
private:
    mutex              d_mutex;
    condition_variable d_condition;
    int                state;
public:
    LockState() {
        this->state = 0;
    }
    void setState(int value) {
        {
            unique_lock<mutex> lock(this->d_mutex);
            state = value;
        }
        this->d_condition.notify_one();
    }
    void wait() {
        unique_lock<mutex> lock(this->d_mutex);
        this->d_condition.wait(lock, [=]{ return state != 0; });
    }
    int getState(){
        return this->state;
    }

};
#endif //ENOBIORECORD_LOCK_STATE_H
