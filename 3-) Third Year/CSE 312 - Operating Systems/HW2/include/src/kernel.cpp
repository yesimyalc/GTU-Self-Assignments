
#include <common/types.h>
#include <gdt.h>
#include <hardwarecommunication/interrupts.h>
#include <hardwarecommunication/pci.h>
#include <drivers/driver.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>
#include <drivers/vga.h>
#include <gui/desktop.h>
#include <gui/window.h>
#include <multitasking.h>
#include <mysyscalls.h>
#include <ProducerConsumer.h>
#include <memorymanagement.h>


// #define GRAPHICSMODE


using namespace myos;
using namespace myos::common;
using namespace myos::drivers;
using namespace myos::hardwarecommunication;
using namespace myos::gui;

TaskManager taskManager;
GlobalDescriptorTable gdt;
InterruptManager interrupts(0x20, &gdt, &taskManager);
uint32_t* memupper = (uint32_t*)(((size_t)multiboot_structure) + 8);
MMU memoryManagementUnit(4, 16, (*memupper)*1024);

void printf(char* str)
{
    static uint16_t* VideoMemory = (uint16_t*)0xb8000;

    static uint8_t x=0,y=0;

    for(int i = 0; str[i] != '\0'; ++i)
    {
        switch(str[i])
        {
            case '\n':
                x = 0;
                y++;
                break;
            default:
                VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | str[i];
                x++;
                break;
        }

        if(x >= 80)
        {
            x = 0;
            y++;
        }

        if(y >= 25)
        {
            for(y = 0; y < 25; y++)
                for(x = 0; x < 80; x++)
                    VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | ' ';
            x = 0;
            y = 0;
        }
    }
}

void printfHex(uint8_t key)
{
    char* foo = "00";
    char* hex = "0123456789ABCDEF";
    foo[0] = hex[(key >> 4) & 0xF];
    foo[1] = hex[key & 0xF];
    printf(foo);
}




class PrintfKeyboardEventHandler : public KeyboardEventHandler
{
public:
    void OnKeyDown(char c)
    {
        char* foo = " ";
        foo[0] = c;
        printf(foo);
    }
};

class MouseToConsole : public MouseEventHandler
{
    int8_t x, y;
public:

    MouseToConsole()
    {
        uint16_t* VideoMemory = (uint16_t*)0xb8000;
        x = 40;
        y = 12;
        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                            | (VideoMemory[80*y+x] & 0xF000) >> 4
                            | (VideoMemory[80*y+x] & 0x00FF);
    }

    virtual void OnMouseMove(int xoffset, int yoffset)
    {
        static uint16_t* VideoMemory = (uint16_t*)0xb8000;
        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                            | (VideoMemory[80*y+x] & 0xF000) >> 4
                            | (VideoMemory[80*y+x] & 0x00FF);

        x += xoffset;
        if(x >= 80) x = 79;
        if(x < 0) x = 0;
        y += yoffset;
        if(y >= 25) y = 24;
        if(y < 0) y = 0;

        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                            | (VideoMemory[80*y+x] & 0xF000) >> 4
                            | (VideoMemory[80*y+x] & 0x00FF);
    }

};

/*Creates a thread using the given newTask
This newTask is added to the TaskManager and given an ID*/
int thread_create(Task* newTask)
{
    return taskManager.AddTask(newTask);
}

//extern "C" int int_bottom();
/*Initiates an interrupt to change the running thread
The following thread according to the Round Robin schedule is run instead
The old thread is put back to the Round Robin queue*/
void thread_yield()
{
    //int_bottom();
    interrupts.interruptOccured=0;
    while(interrupts.interruptOccured==0){}
}


/*Blocks until the thread with the given threadID is terminated
thread_yield is called right after the block is placed to let the following
thread to run instead.
Return false if block fails then thread_yield does not happen.*/
bool thread_join(int threadID)
{
    if(taskManager.joinThread(threadID))
        thread_yield();
}

/*Terminates the thread.
thread_yield is called right after the block is placed to let the following
thread to run instead.*/
void thread_exit()
{
    taskManager.terminateRunningTask();
    thread_yield();
}

/*A thread that will run forever. It will print A.
After it runs for once, it will be blocked until taskB terminates using thread_join()*/
void taskA()
{
	int counter=0;
	while(true)
	{
        printf("A\n");
        counter++;
        if(counter==1)
            thread_join(1);
	}
	thread_exit();
}

/*A thread that will run till the counter is 1000000. It will print B.
The thread will try to use thread_join() on taskA.
However, taskA will have used thread_join() on taskB before and to prevent deadlock, taskB's thread_join() will fail
Vice versa might happen instead as well, it depends on the scheduling during runtime*/
void taskB()
{
    int counter=0;
	while(true)
	{
        printf("B\n");
        counter++;
        if(counter==100)
            thread_join(0);
        if(counter==100000)
            thread_exit();
	}
	thread_exit();
}

/*A thread that will run forever. It will print CCCCCCCC.
The thread will be blocked until threadD is terminated after counter is 100*/
void taskC()
{
	int counter=0;
	while(true)
	{
        printf("CCCCCCCC\n");
        counter++;
        if(counter==100)
            thread_join(3);
	}
	thread_exit();
}

//A thread that will run till the counter is 100000. It will print DDDDDDDDD.
void taskD()
{
    int counter=0;
	while(true)
	{
        printf("DDDDDDDDD\n");
        counter++;
        if(counter==100000)
            thread_exit();
	}
	thread_exit();
}

void test()
{
    uint32_t retrievedItem;
    for(int i=0; i<250; i++)
    {
        /*memoryManagementUnit.insertItem(i);
        retrievedItem=memoryManagementUnit.getItem(i);
        printf("Retrieved: ");
        printfHex(retrievedItem);
        printf("\n");*/

        printf("sdf\n");
    }
}

typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors()
{
    for(constructor* i = &start_ctors; i != &end_ctors; i++)
        (*i)();
}



extern "C" void kernelMain(const void* multiboot_structure, uint32_t /*multiboot_magic*/)
{
    printf("Hello World! --- http://www.AlgorithMan.de\n");

    /*//Creating and adding to the task manager all the threads
    Task task1(&gdt, taskA);
    Task task2(&gdt, taskB);
    Task task3(&gdt, taskC);
    Task task4(&gdt, taskD);
    thread_create(&task1);
    thread_create(&task2);
    thread_create(&task3);
    thread_create(&task4);

    ProducerConsumerThread pair1;
    Task taskProducer1(&gdt, pair1.producerThreadA);
    Task taskConsumer1(&gdt, pair1.consumerThreadA);
    Task taskProducer2(&gdt, pair1.producerThreadB);
    Task taskConsumer2(&gdt, pair1.consumerThreadB);
    thread_create(&taskProducer1);
    thread_create(&taskConsumer1);
    thread_create(&taskProducer2);
    thread_create(&taskConsumer2);*/

    Task task1(&gdt, test);
    thread_create(&task1);

    printf("Initializing Hardware, Stage 1\n");

    #ifdef GRAPHICSMODE
        Desktop desktop(320,200, 0x00,0x00,0xA8);
    #endif

    DriverManager drvManager;

        #ifdef GRAPHICSMODE
            KeyboardDriver keyboard(&interrupts, &desktop);
        #else
            PrintfKeyboardEventHandler kbhandler;
            KeyboardDriver keyboard(&interrupts, &kbhandler);
        #endif
        drvManager.AddDriver(&keyboard);


        #ifdef GRAPHICSMODE
            MouseDriver mouse(&interrupts, &desktop);
        #else
            MouseToConsole mousehandler;
            MouseDriver mouse(&interrupts, &mousehandler);
        #endif
        drvManager.AddDriver(&mouse);

        PeripheralComponentInterconnectController PCIController;
        PCIController.SelectDrivers(&drvManager, &interrupts);

        VideoGraphicsArray vga;

    printf("Initializing Hardware, Stage 2\n");
        drvManager.ActivateAll();

    printf("Initializing Hardware, Stage 3\n");

    #ifdef GRAPHICSMODE
        vga.SetMode(320,200,8);
        Window win1(&desktop, 10,10,20,20, 0xA8,0x00,0x00);
        desktop.AddChild(&win1);
        Window win2(&desktop, 40,15,30,30, 0x00,0xA8,0x00);
        desktop.AddChild(&win2);
    #endif


    interrupts.Activate();

    while(1)
    {
        #ifdef GRAPHICSMODE
            desktop.Draw(&vga);
        #endif
    }
}
