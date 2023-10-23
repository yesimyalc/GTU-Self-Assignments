#ifndef ITIMER_H_INCLUDED
#define ITIMER_H_INCLUDED

#include <chrono>
#include <functional>

using CLOCK = std::chrono::high_resolution_clock;
using TTimerCallback = std::function<void()>;
using Millisecs = std::chrono::milliseconds;
using Timepoint = CLOCK::time_point;
using TPredicate = std::function<bool()>;

class ITimer {
    public:
        // run the callback once at time point tp
        virtual void registerTimer(const Timepoint& tp, const TTimerCallback& cb) = 0;

        // run the callback periodically forever. The first call will be executed after the first period
        virtual void registerTimer(const Millisecs& period, const TTimerCallback& cb) = 0;

        // Run the callback periodically until time point tp The first call will be executed after the first period
        virtual void registerTimer(const Timepoint& tp, const Millisecs& period, const TTimerCallback& cb) = 0;

        //Run the callback periodically. After calling the callback every time, call the predicate to check if the
        //termination criterion is satisfied. If the predicate returns false, stop calling the callback
        virtual void registerTimer(const TPredicate& pred, const Millisecs& period, const TTimerCallback& cb) = 0;
};

#endif
