
#include <multitasking.h>

using namespace myos;
using namespace myos::common;

Task::Task(GlobalDescriptorTable *gdt, void entrypoint())
{
    cpustate = (CPUState*)(stack + 4096 - sizeof(CPUState));

    cpustate -> eax = 0;
    cpustate -> ebx = 0;
    cpustate -> ecx = 0;
    cpustate -> edx = 0;

    cpustate -> esi = 0;
    cpustate -> edi = 0;
    cpustate -> ebp = 0;

    /*
    cpustate -> gs = 0;
    cpustate -> fs = 0;
    cpustate -> es = 0;
    cpustate -> ds = 0;
    */

    // cpustate -> error = 0;

    // cpustate -> esp = ;
    cpustate -> eip = (uint32_t)entrypoint;
    cpustate -> cs = gdt->CodeSegmentSelector();
    // cpustate -> ss = ;
    cpustate -> eflags = 0x202;

    taskState=1;
    waitingThread=-1;
    threadID=0;
}

Task::~Task()
{
}

/*Returns the taskState
taskState is an integer
-1 if terminated, 0 if blocked, 1 if ready, 2 if running*/
int Task::getTaskState()
{
    return taskState;
}

//Changes the taskState of the tast to -1
void Task::terminate()
{
    taskState=-1;
}



TaskManager::TaskManager()
{
    numTasks = 0;
    currentTask = -1;
    totalTaskCount=0;
    blockedTasks=0;
}

TaskManager::~TaskManager()
{
}

/*Adds a thread to the tasks of the TaskManager
Sets the thread's ID accordingly
Increments necessary counters*/
int TaskManager::AddTask(Task* task)
{
    if(numTasks >= 256)
        return false;
    task->threadID=totalTaskCount;  //Assign an actual id
    totalTaskCount++;
    tasks[numTasks++] = task;
    return task->threadID;
}

/*Uses a Round Robin algorithm to schedule the threads
Skips to the following thread
Removes any terminated threads from the tasks*/
CPUState* TaskManager::Schedule(CPUState* cpustate)
{
    if(numTasks <= 0 || blockedTasks+terminatedTasks==numTasks)
        return cpustate;

    if(currentTask >= 0)
        tasks[currentTask]->cpustate = cpustate;

    while(true) //Repeat Round Robin until finding a ready thread
    {
        if(++currentTask >= numTasks)   //If the end is reached, return to the start
            currentTask %= numTasks;

        if(tasks[currentTask]->getTaskState()==-1)  //Remove any terminated threads
        {
            for(int i=currentTask; i<numTasks-1; i++)
                tasks[i]=tasks[i+1];
            numTasks--;
            terminatedTasks--;
        }
        if(tasks[currentTask]->getTaskState()==1)   //If the state is ready, it can be chosen
            break;
    }

    return tasks[currentTask]->cpustate;
}

/*Terminates the current thread
Unblocks any thread waiting for this thread to be terminated*/
void TaskManager::terminateRunningTask()
{
    if(numTasks==0)
        return;

    for(int i=0; i<numTasks; ++i)   //Unblock any blocked threads caused by the current thread
        if(tasks[i]->waitingThread==tasks[currentTask]->threadID)
        {
            tasks[i]->taskState=1;
            tasks[i]->waitingThread=-1;
            blockedTasks--;
        }
    terminatedTasks++;
    tasks[currentTask]->terminate();
}

/*Blocks the current thread until the thread with the given threadID terminates
If the given threadID is the same ad the current thread's ID returns false, does not block
If the given threadID does not belong to any threads returns false, does not block
If the given threadID belongs to a terminated thread returns false, does not block
If the thread with the given threadID is blocked because of the current thread returns false, does not block
If successful, returns true*/
bool TaskManager::joinThread(int threadID)
{
    if(threadID>=totalTaskCount || threadID<0 || threadID==tasks[currentTask]->threadID)
        return false;

    for(int i=0; i<numTasks; ++i)
    {
        if(tasks[i]->threadID==threadID && tasks[i]->getTaskState()!=-1 && isBlockable(i)) //Checks if the found thread is a valid one to have the current task blocked
            break;
        else if(i==numTasks-1) //If a valid thread can't be found return false
            return false;
    }

    //Set blocked state
    tasks[currentTask]->taskState=0;
    tasks[currentTask]->waitingThread=threadID;
    blockedTasks++;

    return true;
}

/*A function to check if putting a block on the currentTask with the visitedIndex is possible
The scenarios like taskA blocks taskB blocks taskC blocks taskA needs to be prevented for deadlocks
Returns true if blockable, false if not blockable*/
bool TaskManager::isBlockable(int visitedIndex)
{
    if(visitedIndex==currentTask)
        return false;
    else if(tasks[visitedIndex]->getTaskState()==0)
    {
        int waitedIndex;
        for(int i=0; i<numTasks; ++i)
            if(tasks[i]->threadID==tasks[visitedIndex]->waitingThread)
                waitedIndex=i;

        return isBlockable(waitedIndex);
    }
    else
        return true;
}

int TaskManager::getCurrentTask()
{
    return currentTask;
}





