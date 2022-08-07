
#ifndef __MYOS__MULTITASKING_H
#define __MYOS__MULTITASKING_H

#include <common/types.h>
#include <gdt.h>

namespace myos
{

    struct CPUState
    {
        common::uint32_t eax;
        common::uint32_t ebx;
        common::uint32_t ecx;
        common::uint32_t edx;

        common::uint32_t esi;
        common::uint32_t edi;
        common::uint32_t ebp;

        /*
        common::uint32_t gs;
        common::uint32_t fs;
        common::uint32_t es;
        common::uint32_t ds;
        */
        common::uint32_t error;

        common::uint32_t eip;
        common::uint32_t cs;
        common::uint32_t eflags;
        common::uint32_t esp;
        common::uint32_t ss;
    } __attribute__((packed));


    class Task
    {
    friend class TaskManager;
    private:
        common::uint8_t stack[4096]; // 4 KiB
        CPUState* cpustate;
        int threadID;   //Unique thread ID
        int taskState; //-1 if terminated, 0 if blocked, 1 if ready, 2 if running
        int waitingThread;  //The ID of the thread that is being waited to terminate (for unblocking). -1 if none (not blocked)
    public:
        Task(GlobalDescriptorTable *gdt, void entrypoint());
        ~Task();
        int getTaskState(); //Return the taskState
        void terminate();   //Set the state as terminated
    };


    class TaskManager
    {
    private:
        Task* tasks[256];
        int numTasks;       //Amount of currently active tasks
        int blockedTasks;   //Count of current blocked tasks
        int terminatedTasks;//Count of terminated tasks that needs to be cleaned
        int currentTask;    //Index of the current running task
        int totalTaskCount; //Amount of created tasks so far
    public:
        TaskManager();
        ~TaskManager();
        int getCurrentTask();
        int AddTask(Task* task);    //Add a new task into the task manager
        CPUState* Schedule(CPUState* cpustate); //Schedule the tasks order
        void terminateRunningTask();    //Terminate the currently running task
        bool joinThread(int threadID);  //Block the current task until the task with the given id terminates
        bool isBlockable(int visitedIndex); //Checks if the current task can be blocked with a specific thread
    };



}


#endif
