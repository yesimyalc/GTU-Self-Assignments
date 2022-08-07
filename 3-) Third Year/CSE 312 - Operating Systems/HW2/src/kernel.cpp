
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
#include <math.h>


// #define GRAPHICSMODE


using namespace myos;
using namespace myos::common;
using namespace myos::drivers;
using namespace myos::hardwarecommunication;
using namespace myos::gui;

TaskManager taskManager;
GlobalDescriptorTable gdt;
InterruptManager interrupts(0x20, &gdt, &taskManager);
MMU memoryManagementUnit;
char chosenPageAlgo='0';
char chosenSortAlgo='0';
char selectedChar='-';
int chosenPageAlgoInt=0;
int chosenSortAlgoInt=0;
void* multibootS;

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
        selectedChar=foo[0];
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
    while(true){}
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

//This was for testing the storage insertion and retrieval.
void test()
{
    uint32_t retrievedItem;
    uint32_t returnVal;
    for(int i=0; i<250; i++)
    {
        returnVal=memoryManagementUnit.insertItem(i);
        if(returnVal==-1)
        {
            printf("Insertion failed.\n");
            thread_exit();
        }
        retrievedItem=memoryManagementUnit.getItem(i);
        if(retrievedItem==-1)
        {
            printf("Retrieval failed.\n");
            thread_exit();
        }

        printfHex(retrievedItem);
        printf(", ");
    }

    printf("\nTotal Misses: ");
    printfHex(memoryManagementUnit.missCounter+memoryManagementUnit.totalMiss);
    printf("\nTotal Hits: ");
    printfHex(memoryManagementUnit.hitCounter+memoryManagementUnit.totalHit);
    printf("\n");
    printf("\n");

    for(int i=0; i<250; i++)
    {
        returnVal=memoryManagementUnit.insertItem(i);
        if(returnVal==-1)
        {
            printf("Insertion failed.\n");
            thread_exit();
        }
        retrievedItem=memoryManagementUnit.getItem(i);
        if(retrievedItem==-1)
        {
            printf("Retrieval failed.\n");
            thread_exit();
        }

        printfHex(retrievedItem);
        printf(", ");
    }

    thread_exit();
}

//Bubblesort algorithm
void bubble(int* arr, int n)
{
    int i, j;
    for(i=0; i<n-1; i++)
        for(j=0; j<n-i-1; j++)
            if(memoryManagementUnit.getItem(j)>memoryManagementUnit.getItem(j+1))
            {
                int temp=memoryManagementUnit.getItem(j+1);
                int temp2=memoryManagementUnit.getItem(j);

                memoryManagementUnit.changeVirtualAddress(j, j+1);

                arr[j+1]=temp2;
                arr[j]=temp;
            }
}

int partition(int arr[], int start, int end)
{
    int pivot=memoryManagementUnit.getItem(start);

    int count=0;
    for(int i=start+1; i<=end; i++)
        if(memoryManagementUnit.getItem(i)<=pivot)
            count++;

    int pivotIndex=start+count;

    int temp=memoryManagementUnit.getItem(pivotIndex);
    int temp2=memoryManagementUnit.getItem(start);
    memoryManagementUnit.changeVirtualAddress(pivotIndex, start);
    arr[pivotIndex]=temp2;
    arr[start]=temp;

    int i=start, j=end;
    while(i<pivotIndex && j>pivotIndex)
    {
        while(memoryManagementUnit.getItem(i)<=pivot)
            i++;

        while(memoryManagementUnit.getItem(j)>pivot)
            j--;

        if(i<pivotIndex && j>pivotIndex)
        {
            temp=memoryManagementUnit.getItem(i);
            temp2=memoryManagementUnit.getItem(j);
            memoryManagementUnit.changeVirtualAddress(i, j);
            arr[i]=temp2;
            arr[j]=temp;
            i++;
            j--;
        }
    }

    return pivotIndex;
}

//Quicksort algorithm
void quick(int* arr, int start, int end)
{
    if(start>=end)
        return;

    int p=partition(arr, start, end);
    quick(arr, start, p-1);
    quick(arr, p+1, end);
}

//Insertion sort algorithm
void insertion(int* arr, int n)
{
    int i, key, j;
    for(i=1; i<n; i++)
    {
        key=memoryManagementUnit.getItem(i);
        j=i-1;

        while(j>=0 && memoryManagementUnit.getItem(j)>key)
        {
            int temp=memoryManagementUnit.getItem(j+1);
            int temp2=memoryManagementUnit.getItem(j);
            memoryManagementUnit.changeVirtualAddress(j+1, j);
            arr[j+1]=temp2;
            arr[j]=temp;
            j=j-1;
        }
    }
}

//algorithm=0 means bubble, algorithm=1 means quick, algorithm=2 means insertion
//sorted=0 means not sorted, sorted=1 means sorted
//If the sizeRespectToMem is lower than %100 then it means the disk space cannot be utilized.
//To be able to show page replacements, if the condition above happens, exception occurs.
//The chosen algorithm is performed and the outcomes are reported.
void sortFunction(int* arr, int sizeRespectToMem, int algorithm, int sorted)
{
    if(sizeRespectToMem<=100)
    {
        printf("The array size is already smaller than the physical memory size(RAM). There is no way to observe page replacements.");
        thread_exit();
    }

    int realSize=(sizeRespectToMem*memoryManagementUnit.ram.ramPageTable.pageC)/100+1;

    if(algorithm==0)
        bubble(arr, realSize);
    else if(algorithm==1)
        quick(arr, 0, realSize-1);
    else
        insertion(arr, realSize);

    printf("\nOutcome Array\n");
    for(int i=0; i<realSize; ++i)
    {
        printfHex(arr[i]);
        printf(", ");
    }
    printf("\n");

    printf("\nTotal Misses: ");
    printfHex(memoryManagementUnit.missCounter+memoryManagementUnit.totalMiss);
    printf("\nTotal Hits: ");
    printfHex(memoryManagementUnit.hitCounter+memoryManagementUnit.totalHit);
    printf("\nMiss Rate: %");
    int misses=memoryManagementUnit.missCounter+memoryManagementUnit.totalMiss;
    int hits=memoryManagementUnit.hitCounter+memoryManagementUnit.totalHit;
    int hitRate=(100*hits)/(misses+hits);
    int missRate=(100*misses)/(misses+hits);
    printfHex(missRate);
    printf("\nHit Rate: %");
    printfHex(hitRate);
    printf("\nPages Loaded & Pages Written Back: ");
    printfHex(misses);
    printf("\n");

}

//A thread for sort operations
//User is asked to choose the paging algorithm they want to use and the sorting algorithm they want to use
//If they enter invalid choices, default of 0,0 will be used meaning FIFO and Bubble Sort
//The array is set and loaded into the storage
//I have chosen the RAM size as 1 MB and Disk size as 16 MB
//Therefore I have calculated that RAM can hold at most 51 blocks.
//To be able to utilize both RAM and Disk I have created a randomized array with size 250.
//If there is not enough space in the storage for the RAM, the thread exits.
//The size is calculated according to the RAM size and sortFunction is called.
//Note that eventhough RAM size is 1MB here, I treated it as being 51. Because RAM can hold at max 51 blocks.
void sortStart()
{
    printf("Choose the paging algorithm:\n");
    printf("Enter 0 for FIFO\n");
    printf("Enter 1 for Second Chance\n");
    printf("Enter 2 for LRU\n");
    while(selectedChar=='-'){}
    chosenPageAlgo=selectedChar;
    selectedChar='-';

    printf("\nChoose the sorting algorithm:\n");
    printf("Enter 0 for Bubble Sort\n");
    printf("Enter 1 for Quick Sort\n");
    printf("Enter 2 for Insertion Sort\n");
    while(selectedChar=='-'){}
    chosenSortAlgo=selectedChar;

    printf("\n");

    if(chosenPageAlgo=='0')
        chosenPageAlgoInt=0;
    else if(chosenPageAlgo=='1')
        chosenPageAlgoInt=1;
    else if(chosenPageAlgo=='2')
        chosenPageAlgoInt=2;
    else
        printf("Wrong input for page algorithm. Default 0 will be used.\n");

    if(chosenSortAlgo=='0')
        chosenSortAlgoInt=0;
    else if(chosenSortAlgo=='1')
        chosenSortAlgoInt=1;
    else if(chosenSortAlgo=='2')
        chosenSortAlgoInt=2;
    else
        printf("Wrong input for sort algorithm. Default 0 will be used.\n");

    uint32_t* memupper = (uint32_t*)(((size_t)multibootS) + 8);
    memoryManagementUnit.setMMU(1, 16, (*memupper)*1024, chosenPageAlgoInt);

    int sortingArray[250]={218,	96,	234, 81, 195, 161, 247, 57, 121, 68, 134, 38, 166, 161, 66, 189, 23, 236, 188, 214, 84, 241, 80, 3, 30,
        90, 191, 107, 78, 86, 63, 37, 94, 205, 179, 55, 32, 110, 233, 115, 234, 53, 55, 111, 52, 188, 137, 109, 8, 133,
        121, 62, 53, 217, 195, 44, 238, 11, 193, 24, 203, 95, 40, 115, 40, 131, 194, 218, 105, 241, 150, 186, 146, 240, 148,
        34, 52, 74, 241, 23, 57, 185, 124, 133, 216, 136, 98, 189, 75, 219, 128, 168, 64, 239, 28, 19, 114, 160, 228, 142,
        85, 18, 160, 7, 83, 208, 115, 18, 68, 69, 207, 235, 228, 221, 13, 52, 1, 217, 221, 105, 193, 226, 113, 181, 147,
        220, 139, 153, 140, 103, 158, 12, 191, 64, 189, 111, 69, 249, 136, 176, 217, 224, 121, 121, 185, 23, 110, 162, 236, 45,
        4, 47, 100, 226, 123, 55, 229, 24, 195, 62, 6, 14, 208, 86, 148, 239, 137, 93, 22, 70, 73, 109, 57, 239, 168,
        245, 29, 127, 62, 26, 208, 186, 233, 91, 10, 154, 219, 85, 43, 13, 189, 167, 163, 195, 52, 1, 164, 46, 116, 99,
        233, 79, 180, 36, 167, 59, 219, 214, 237, 73, 167, 131, 66, 246, 163, 156, 154, 48, 108, 136, 238, 3, 156, 49, 185,
        246, 50, 139, 69, 121, 15, 192, 120, 220, 67, 191, 224, 158, 106, 78, 54, 126, 175, 144, 148, 145, 246, 171, 105, 92
    };

    for(int i=0; i<250; ++i)
    {
        int returnVal=memoryManagementUnit.insertItem(sortingArray[i]);
        if(returnVal==-1)
        {
            printf("There is not enough space for the array in the disk.\n");
            thread_exit();
        }
    }

    printf("\nInitial Array\n");
    for(int i=0; i<250; ++i)
    {
        printfHex(memoryManagementUnit.getItem(i));
        printf(", ");
    }
    printf("\n");

    int calcSize=((250*100)/memoryManagementUnit.ram.ramPageTable.pageC);
    sortFunction(sortingArray, calcSize, chosenSortAlgoInt, 0);

    thread_exit();
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
    multibootS=(void*)multiboot_structure;

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

    /*Task task1(&gdt, test);
    thread_create(&task1);*/



    Task task2(&gdt, sortStart);
    thread_create(&task2);

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
