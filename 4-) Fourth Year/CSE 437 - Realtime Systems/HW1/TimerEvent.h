#ifndef TIMEREVENT_H_INCLUDED
#define TIMEREVENT_H_INCLUDED

#include <memory>
#include <chrono>
#include <functional>

using CLOCK = std::chrono::high_resolution_clock;
using TTimerCallback = std::function<void()>;
using Millisecs = std::chrono::milliseconds;
using Timepoint = CLOCK::time_point;
using TPredicate = std::function<bool()>;

class TimerEvent
{
    public:
        //Constructors
        TimerEvent(const TTimerCallback& cb, const Timepoint& tp);  //case 1
        TimerEvent(const TTimerCallback& cb, const Millisecs& period);  //case 2
        TimerEvent(const TTimerCallback& cb, const Timepoint& tp, const Millisecs& period); //case 3
        TimerEvent(const TTimerCallback& cb, const Millisecs& period, const TPredicate& pred);  //case 4

        //getters
        const Timepoint& getNextRunTime()const{return nextRunTime;}

        //Public Functions
        //Runs the event callback function
        //Returns false if it is the last run of the event, returns true if the event will be run later again
        bool runEvent();

    private:
        TTimerCallback eventCallback{};
        Timepoint eventTp{};
        Millisecs eventPeriod{};
        TPredicate eventPredicate{};

        //The timepoint that the event should run next
        Timepoint nextRunTime{};
        //Total count of how many times the event has run before
        int runCount=0;
        //The initialization case of the event (depends on which constructor it was initialized with)
        int eventCase=0;

        //Calculates the next run time of the event
        //Returns false if it is the last run of the event, returns true if the event will be run later again
        bool calculateNextRunTime();
};

#endif
