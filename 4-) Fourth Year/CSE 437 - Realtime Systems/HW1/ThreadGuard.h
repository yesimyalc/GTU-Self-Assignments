#ifndef THREADGUARD_H_INCLUDED
#define THREADGUARD_H_INCLUDED

#include <iostream>

/*
A class that guards a thread
The guarded thread is joined automatically when the class object dies
Regular copy and assignment is not allowed
*/
class ThreadGuard
{
    public:
        std::thread t;

        ThreadGuard(std::thread&& t):t(std::move(t)){};
        ~ThreadGuard()
        {
            if(t.joinable())
                t.join();
        }

        ThreadGuard(ThreadGuard&)=delete;
        ThreadGuard& operator=(ThreadGuard&)=delete;

        ThreadGuard(ThreadGuard&& other)
        {
            t=std::move(other.t);
        }

};

#endif
