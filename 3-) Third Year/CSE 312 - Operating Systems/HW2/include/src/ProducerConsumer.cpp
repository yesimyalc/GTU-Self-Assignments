
#include <common/types.h>
#include <multitasking.h>
#include <mysyscalls.h>
#include <ProducerConsumer.h>

using namespace myos;
using namespace myos::common;

int ProducerConsumerThread::turnA=-1;
int ProducerConsumerThread::itemCountA=0;
int ProducerConsumerThread::interestedA[]={0,0};
int ProducerConsumerThread::producerSetA=0;
int ProducerConsumerThread::consumerSetA=0;
int ProducerConsumerThread::itemsListA[]={};

int ProducerConsumerThread::turnB=-1;
int ProducerConsumerThread::itemCountB=0;
int ProducerConsumerThread::interestedB[]={0,0};
int ProducerConsumerThread::producerSetB=0;
int ProducerConsumerThread::consumerSetB=0;
int ProducerConsumerThread::itemsListB[]={};

void printf(char* str);
void printfHex(uint8_t);

ProducerConsumerThread::ProducerConsumerThread()
{
}

ProducerConsumerThread::~ProducerConsumerThread()
{
}

/* This is the producer thread for the pairA
It checks if there is a currently running producerA thread.
If there is not, it saves itself as running
If there is, the thread is terminated not to let to have more than 1 producerA thread
In an infinite loop tries to increment the count of items.
This is equal to producing an item.
Producing an item is considered as a critical operation therefore before producing an item,
uses enterRegionA function to check if it is eligible to enter the critical region.
Uses leaveRegionA to free the critical region*/
void ProducerConsumerThread::producerThreadA()
{
    if(producerSetA==0)
        producerSetA=1;
    else
        thread_exit();

    while(true)
    {
        printf("ProducerA wanting to enter the critical region.\n");
        enterRegionA('p');  //Critical region start
        printf("ProducerA is in the critical region.\n");
        itemsListA[itemCountA++]=1;
        printf("Current item count A: ");
        printfHex(itemCountA);
        printf("\n");
        leaveRegionA('p');  //Critical region end
    }

    producerSetA=0;
    thread_exit();
}

/* This is the consumer thread for pairA
It checks if there is a currently running consumerA thread.
If there is not, it saves itself as running
If there is, the thread is terminated not to let to have more than 1 consumerA thread
In an infinite loop tries to decrement the count of items.
This is equal to consuming an item.
Consuming and item is considered as a critical operation therefore before consuming an item,
uses enterRegionA function to check if it is eligible to enter the critical region.
Uses leaveRegionA to free the critical region*/
void ProducerConsumerThread::consumerThreadA()
{
    if(consumerSetA==0)
        consumerSetA=1;
    else
        thread_exit();

    while(true)
    {
        printf("CustomerA wanting to enter the critical region.\n");
        enterRegionA('c'); //Critical region start
        printf("ConsumerA is in the critical region.\n");
        itemsListA[--itemCountA]=0;
        printf("Current item count A: ");
        printfHex(itemCountA);
        printf("\n");
        leaveRegionA('c'); //Critical region end
    }

    consumerSetA=0;
    thread_exit();
}

/*Checks if the called thread is eligible to enter the critical region
Checks are done for pairA
Uses Peterson's Algorithm to do checks
If the parameter identity is p then the caller is a producer
If the parameter identity is c then the caller is a costumer
If it's none of them, it is an invalid identity
If producer, does busy waiting until there is enough space to produce items.
If consumer, does busy waiting until there is products to consume.
Gives the turnA to 0 if producer, to 1 if consumer.
Sets interestedA[0]=1 if producer, interestedA[1]=1 if consumer
If the caller has the turnA but the other one is also interested, does busy waiting
When all the requirements are checked, enters the critical region.*/
void ProducerConsumerThread::enterRegionA(char identity)
{
    int otherIndex;
    int processIndex;
    int i=0;

    if(identity=='p')
        processIndex=0;
    else if(identity=='c')
        processIndex=1;
    else
        thread_exit();
    otherIndex=1-processIndex;

    //Item availabilty check
    if(processIndex==0)
        while(itemCountA==MAX_SIZE)
        {
            if(i%200000==0) //Modulo is used not to print this statement so many times.
                printf("ProducerA: There is no space for a new product.\n");
            i++;
        }
    else
        while(itemCountA==0)
        {
            if(i%200000==0)
                printf("ConsumerA: There is no product to consume.\n");
            i++;
        }

    interestedA[processIndex]=1;
    turnA=processIndex;

    //Critical region eligibility check
    while(turnA==processIndex && interestedA[otherIndex]==1)
    {
        if(i%200000==0 && processIndex==0)
            printf("ProducerA: CustomerA is in the critical region. Cannot enter critical region.\n");
        else if(i%200000==0 && processIndex==1)
            printf("CustomerA: ProducerA is in the critical region. Cannot enter critical region.\n");
        i++;
    }
}

/*Exits the critical region for pair A
If the parameter identity is p then the caller is a producer
If the parameter identity is c then the caller is a costumer
If it's none of them, it is an invalid identity
If producer, sets interestedA[0]=0
If consumer, sets interestedA[1]=0 so that others can enter the critical region.*/
void ProducerConsumerThread::leaveRegionA(char identity)
{
    int processIndex;
    if(identity=='p')
        processIndex=0;
    else if(identity=='c')
        processIndex=1;
    else
        return;
    printf("A Pair: Leaving critical region soon.\n");
    interestedA[processIndex]=0;     //Freeing the critical region
}



/* This is the producer thread for the pairB
It checks if there is a currently running producerB thread.
If there is not, it saves itself as running
If there is, the thread is terminated not to let to have more than 1 producerB thread
In an infinite loop tries to increment the count of items.
This is equal to producing an item.
Producing an item is considered as a critical operation therefore before producing an item,
uses enterRegionB function to check if it is eligible to enter the critical region.
Uses leaveRegionB to free the critical region*/
void ProducerConsumerThread::producerThreadB()
{
    if(producerSetB==0)
        producerSetB=1;
    else
        thread_exit();

    while(true)
    {
        printf("ProducerB wanting to enter the critical region.\n");
        enterRegionB('p'); //Critical region start
        printf("ProducerB is in the critical region.\n");
        itemsListB[itemCountB++]=1;
        printf("Current item count B: ");
        printfHex(itemCountB);
        printf("\n");
        leaveRegionB('p'); //Critical region end
    }

    producerSetB=0;
    thread_exit();
}

/* This is the consumer thread for pairB
It checks if there is a currently running consumerB thread.
If there is not, it saves itself as running
If there is, the thread is terminated not to let to have more than 1 consumerB thread
In an infinite loop tries to decrement the count of items.
This is equal to consuming an item.
Consuming and item is considered as a critical operation therefore before consuming an item,
uses enterRegionB function to check if it is eligible to enter the critical region.
Uses leaveRegionB to free the critical region*/
void ProducerConsumerThread::consumerThreadB()
{
    if(consumerSetB==0)
        consumerSetB=1;
    else
        thread_exit();

    while(true)
    {
        printf("CustomerB wanting to enter the critical region.\n");
        enterRegionB('c'); //Critical region start
        printf("ConsumerB is in the critical region.\n");
        itemsListB[--itemCountB]=-1;
        printf("Current item count B: ");
        printfHex(itemCountB);
        printf("\n");
        leaveRegionB('c'); //Critical region end
    }

    consumerSetB=0;
    thread_exit();
}

/*Checks if the called thread is eligible to enter the critical region
Checks are done for pairB
Uses Peterson's Algorithm to do checks
If the parameter identity is p then the caller is a producer
If the parameter identity is c then the caller is a costumer
If it's none of them, it is an invalid identity
If producer, does busy waiting until there is enough space to produce items.
If consumer, does busy waiting until there is products to consume.
Gives the turnB to 0 if producer, to 1 if consumer.
Sets interestedB[0]=1 if producer, interestedB[1]=1 if consumer
If the caller has the turnA but the other one is also interested, does busy waiting
When all the requirements are checked, enters the critical region.*/
void ProducerConsumerThread::enterRegionB(char identity)
{
    int otherIndex;
    int processIndex;
    int i=0;

    if(identity=='p')
        processIndex=0;
    else if(identity=='c')
        processIndex=1;
    else
        thread_exit();
    otherIndex=1-processIndex;

    //Item availabilty check
    if(processIndex==0)
        while(itemCountB==MAX_SIZE)
        {
            if(i%200000==0) //Modulo is used not to print this statement so many times.
                printf("ProducerB: There is no space for a new product.\n");
            i++;
        }
    else
        while(itemCountB==0)
        {
            if(i%200000==0)
                printf("ConsumerB: There is no product to consume.\n");
            i++;
        }

    interestedB[processIndex]=1;
    turnB=processIndex;

    //Critical region eligibility check
    while(turnB==processIndex && interestedB[otherIndex]==1)
    {
        if(i%200000==0 && processIndex==0)
            printf("ProducerB: CustomerB is in the critical region. Cannot enter critical region.\n");
        else if(i%200000==0 && processIndex==1)
            printf("CustomerB: ProducerB is in the critical region. Cannot enter critical region.\n");
        i++;
    }
}

/*Exits the critical region for pair B
If the parameter identity is p then the caller is a producer
If the parameter identity is c then the caller is a costumer
If it's none of them, it is an invalid identity
If producer, sets interestedB[0]=0
If consumer, sets interestedB[1]=0 so that others can enter the critical region.*/
void ProducerConsumerThread::leaveRegionB(char identity)
{
    int processIndex;
    if(identity=='p')
        processIndex=0;
    else if(identity=='c')
        processIndex=1;
    else
        return;
    printf("B Pair: Leaving critical region soon.\n");
    interestedB[processIndex]=0;    //Freeing the critical region
}

/*This is the version that does not include any critical region checks of producerThreadA
While testing I commented out the version above and ran this one instead.
This one causes race condition*/
/*void ProducerConsumerThread::producerThreadA()
{
    if(producerSetA==0)
        producerSetA=1;
    else
        thread_exit();

    while(true)
    {
        printf("ProducerA wanting to enter the critical region.\n");
        printf("ProducerA is in the critical region.\n");
        itemsListA[itemCountA++]=1;
        printf("Current item count A: ");
        printfHex(itemCountA);
        printf("\n");
        printf("ProducerA is leaving the critical region.\n");
    }

    producerSetA=0;
    thread_exit();
}*/

/* This is the version that does not include any critical region checks of consumerThreadA
While testing I commented out the version above and ran this one instead.
This one causes race condition*/
/*void ProducerConsumerThread::consumerThreadA()
{
    if(consumerSetA==0)
        consumerSetA=1;
    else
        thread_exit();

    while(true)
    {
        printf("CustomerA wanting to enter the critical region.\n");
        printf("ConsumerA is in the critical region.\n");
        itemsListA[--itemCountA]=0;
        printf("Current item count A: ");
        printfHex(itemCountA);
        printf("\n");
        printf("CustomerA is leaving the critical region\n");
    }

    consumerSetA=0;
    thread_exit();
}*/

