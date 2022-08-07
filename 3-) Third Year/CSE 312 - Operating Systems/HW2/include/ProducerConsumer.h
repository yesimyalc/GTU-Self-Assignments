#ifndef __MYOS__PRODUCERCONSUMER_H
#define __MYOS__PRODUCERCONSUMER_H

#include <common/types.h>
#include <multitasking.h>

#define MAX_SIZE 100    //Max size for product count. This will be 64 in hex when printed out

namespace myos
{
    /*A class to control producer-consumer threads.
    It includes pair A and pair B of producers and consumers
    Only one exact pair can work from A and B can run at the same time
    So there cannot be 2 A producers or 2 B producers at the same time.
    However, there can be 1 from A and B at the same time.*/
    class ProducerConsumerThread
    {
    private:
        static int turnA;   //Who has the current turn. 0 if producerA, 1 if consumerA
        static int interestedA[2];  //1 if interested in getting into the critical zone, 0 if not
        static int itemCountA;      //Currently produced A product count
        static int producerSetA;    //0 if there is not an active producerA thread, 1 if there is
        static int consumerSetA;    //0 if there is not an active consumerA thread, 1 if there is
        static int itemsListA[MAX_SIZE];//items list

        static int turnB;   //Who has the current turn. 0 if producerB, 1 if consumerB
        static int interestedB[2];   //1 if interested in getting into the critical zone, 0 if not
        static int itemCountB;      //Currently produced B product count
        static int producerSetB;    //0 if there is not an active producerB thread, 1 if there is
        static int consumerSetB;    //0 if there is not an active consumerB thread, 1 if there is
        static int itemsListB[MAX_SIZE];//items list

        static void leaveRegionA(char identity);    //Leave the critical region for pair A
        static void enterRegionA(char identity);    //Enter the critical region for pair A

        static void leaveRegionB(char identity);    //Leave the critical region for pair B
        static void enterRegionB(char identity);    //Enter the critical region for pair B
    public:
        ProducerConsumerThread();
        ~ProducerConsumerThread();

        static void producerThreadA();  //Producer thread for pair A
        static void consumerThreadA();  //Consumer thread for pair A

        static void producerThreadB();  //Producer thread for pair B
        static void consumerThreadB();  //Consumer thread for pair B
    };
}

#endif
