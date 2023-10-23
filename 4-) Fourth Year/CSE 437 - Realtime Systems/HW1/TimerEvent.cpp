#include <iostream>

#include "TimerEvent.h"

TimerEvent::TimerEvent(const TTimerCallback& cb, const Timepoint& tp)
{
    eventCallback=cb;
    eventTp=tp;

    eventCase=1;
    calculateNextRunTime();
}

TimerEvent::TimerEvent(const TTimerCallback& cb, const Millisecs& period)
{
    eventCallback=cb;
    eventPeriod=period;

    eventCase=2;
    calculateNextRunTime();
}

TimerEvent::TimerEvent(const TTimerCallback& cb, const Timepoint& tp, const Millisecs& period)
{
    eventCallback=cb;
    eventTp=tp;
    eventPeriod=period;

    eventCase=3;
    calculateNextRunTime();
}

TimerEvent::TimerEvent(const TTimerCallback& cb, const Millisecs& period, const TPredicate& pred)
{
    eventCallback=cb;
    eventPeriod=period;
    eventPredicate=pred;

    eventCase=4;
    calculateNextRunTime();
}

bool TimerEvent::calculateNextRunTime()
{
    if(eventCase==2 && runCount!=0) //If it is case 2 and not the first run, the next run time is incremented by the period
        nextRunTime+=eventPeriod;
    else if(eventCase==2)   //If it is case 2 and the first run, the next run time is current+period
        nextRunTime=CLOCK::now()+eventPeriod;
    else if(eventCase==3 && runCount!=0)    //If it is case 3 and not the first run, the next run time is incremented by the period.
    {
        nextRunTime+=eventPeriod;
        if(getNextRunTime()>eventTp)    //If the calculated next run time is beyond eventTp, return false
            return false;
    }
    else if(eventCase==3)   //If it is case 3 and the first run, the next run time is current+period
    {
        nextRunTime=CLOCK::now()+eventPeriod;
        if(getNextRunTime()>eventTp)    //If the calculated next run time is beyond eventTp, return false
            return false;
    }
    else if(eventCase==4 && runCount!=0)    //If it is case 4 and not the first run, the next run time is incremented by the period
    {
        nextRunTime+=eventPeriod;
        if(!eventPredicate())   //If the eventPredicate returns false, return false
            return false;
    }
    else if(eventCase==4)   //If it is the case 4 and the first run, the next run time is current+period
    {
        nextRunTime=CLOCK::now()+eventPeriod;
    }
    else if(eventCase==1 && runCount!=0) //case 1 end
        return false;
    else if(eventCase==1)    //case 1 start
        nextRunTime=eventTp;

    return true;
}

//run the callback, increment the runCount and calculate the next run time
bool TimerEvent::runEvent()
{
    eventCallback();
    runCount++;
    return calculateNextRunTime();
}

