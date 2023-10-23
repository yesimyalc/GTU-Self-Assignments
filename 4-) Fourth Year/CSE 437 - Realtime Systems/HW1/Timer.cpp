#include <iostream>
#include <thread>
#include <memory>

#include "Timer.h"

Timer::Timer()
{
    tg=std::make_unique<ThreadGuard>(ThreadGuard(std::thread(&Timer::threadFunction, this) ));
}

Timer::~Timer()
{
    //If the Timer object is dying, set the endThread flag so that the threadGuard can join the main thread
    endThread=true;
    cv.notify_all();
}

//All register timers create a TimerEvent and then adds it to the eventList queue
void Timer::registerTimer(const Timepoint& tp, const TTimerCallback& cb)
{
    std::lock_guard<std::mutex> lock(queueMutex);
    eventList.push(TimerEvent(cb, tp));
    cv.notify_all();
}

void Timer::registerTimer(const Millisecs& period, const TTimerCallback& cb)
{
    std::lock_guard<std::mutex> lock(queueMutex);
    eventList.push(TimerEvent(cb, period));
    cv.notify_all();
}

void Timer::registerTimer(const Timepoint& tp, const Millisecs& period, const TTimerCallback& cb)
{
    std::lock_guard<std::mutex> lock(queueMutex);
    eventList.push(TimerEvent(cb, tp, period));
    cv.notify_all();
}

void Timer::registerTimer(const TPredicate& pred, const Millisecs& period, const TTimerCallback& cb)
{
    std::lock_guard<std::mutex> lock(queueMutex);
    eventList.push(TimerEvent(cb, period, pred));
    cv.notify_all();
}

void Timer::threadFunction()
{
    while(true)
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        if(eventList.size()==0) //If no TimerEvents were assigned, wait till either there is a end thread signal or if a TimerEvent is assigned
        {
            cv.wait(lock, [&]() {
                if(endThread)
                    return true;
                else if(eventList.size()>prevQueueSize)
                    return true;

                return false;
            });

            if(endThread==true)
                break;

            if(eventList.size()>prevQueueSize)
                 prevQueueSize++;

        }
        else
        {
            //Calculate the duration till the next timer event (the time till the TimerEvent which is at the front of the queue)
            std::chrono::nanoseconds waitTime=std::chrono::duration_cast<std::chrono::nanoseconds>(eventList.top().getNextRunTime()-CLOCK::now());
            cv.wait_for(lock, waitTime, [&]() { //Wake up after waitTime duration or if there is any new events registered or it is time to end the thread
                    if(endThread)
                        return true;
                    else if(eventList.size()>prevQueueSize)
                        return true;

                    return false;
            });

            if(eventList.size()>0 && eventList.top().getNextRunTime()<=CLOCK::now())    //If woke up because its time for a timer event, run the callback
            {
                TimerEvent processingEvent=eventList.top();
                eventList.pop();
                if(processingEvent.runEvent()) //If the event will be run again, push it back into the list
                    eventList.push(processingEvent);
                else
                    prevQueueSize=eventList.size(); //If not, update the prevQueueSize
            }

            if(endThread==true)
                break;

            if(eventList.size()>prevQueueSize)
                 prevQueueSize++;
        }
    }
}




bool operator>(const TimerEvent& a, const TimerEvent& b)
{
    return a.getNextRunTime()>b.getNextRunTime();
}


