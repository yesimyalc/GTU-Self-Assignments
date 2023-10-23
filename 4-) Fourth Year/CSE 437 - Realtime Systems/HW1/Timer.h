#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED

#include <queue>
#include <vector>
#include <atomic>
#include <mutex>
#include <condition_variable>

#include "ITimer.h"
#include "TimerEvent.h"
#include "ThreadGuard.h"

using CLOCK = std::chrono::high_resolution_clock;
using TTimerCallback = std::function<void()>;
using Millisecs = std::chrono::milliseconds;
using Timepoint = CLOCK::time_point;
using TPredicate = std::function<bool()>;

class Timer: public ITimer
{
    public:
        Timer();
        ~Timer();

        // run the callback once at time point tp
        virtual void registerTimer(const Timepoint& tp, const TTimerCallback& cb);

        // run the callback periodically forever. The first call will be executed after the first period
        virtual void registerTimer(const Millisecs& period, const TTimerCallback& cb);

        // Run the callback periodically until time point tp. The first call will be executed after the first period
        virtual void registerTimer(const Timepoint& tp, const Millisecs& period, const TTimerCallback& cb);

        //Run the callback periodically. After calling the callback every time, call the predicate to check if the
        //termination criterion is satisfied. If the predicate returns false, stop calling the callback
        virtual void registerTimer(const TPredicate& pred, const Millisecs& period, const TTimerCallback& cb);

    private:
        //A priority queue that holds assigned TimerEvents. The events are ordered according to their nextRunTime having the earliest one at the front
        std::priority_queue<TimerEvent, std::vector<TimerEvent>, std::greater<TimerEvent> > eventList;
        //Previous size of the queue, used to determine if there is any change in the queue size
        int prevQueueSize=0;
        //A thread guard that holds the main thread that runs timing operations
        std::unique_ptr<ThreadGuard> tg=nullptr;
        //A boolean flag to understand if the main thread should end
        std::atomic_bool endThread{false};
        std::mutex queueMutex;
        std::condition_variable cv;

        //Main thread function
        void threadFunction();
};


#endif
