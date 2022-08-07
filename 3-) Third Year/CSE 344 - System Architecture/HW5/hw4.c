#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <stdbool.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <pthread.h>
#include <sys/ipc.h>
#include "semun.h"
#include <time.h>

static const char default_format[] = "%b %d %Z %Y %H %M %S";

void checkArgc(int count);
int retrieveArguments(int count, char* arguements[], int* consumer, int* n, char** dataPath);
void* consumerFunction(void* args);
void* supplierFunction(void* args);
int openFile(char* filePath, int flags, mode_t mode);
int writeOutput(char* output, int fileFd);
void arrangeSupplierOutput1(char* output, char readItem, int count1, int count2);
void arrangeSupplierOutput2(char* output, char deliveredItem, int count1, int count2);
void arrangeConsumerOutput1(char* output, int no, int iterationNo, int count1, int count2);
void arrangeConsumerOutput2(char* output, int no, int iterationNo, int count1, int count2);
void arrangeConsumerOutput3(char* output, int no);
void sigintHandler(int signum);
void removeSems(void);

int semaphoreSetId;
int sigintFlag=0;
int dataFile;

int main(int argc, char* argv[])
{
    setenv("TZ", "Europe/Istanbul", 1);
    tzset();
    setbuf(stdout, NULL);

    //Checking the command line arguments
    checkArgc(argc);

    //Assigning signal handlers
    //SIGINT
    struct sigaction sigintAct={{0}};
    sigemptyset(&sigintAct.sa_mask);
    sigintAct.sa_handler=&sigintHandler;
    sigaddset(&sigintAct.sa_mask, SIGPIPE);
    if(sigaction(SIGINT, &sigintAct, NULL)==-1)
    {
        perror("Cannot assign the signal handler");
        exit(EXIT_FAILURE);
    }

    //Retrieving the command line inputs
    int consumerCount=-1;
    int nCount=-1;
    char* dataFilePath=NULL;
    retrieveArguments(argc, argv, &consumerCount, &nCount, &dataFilePath);
    if(dataFilePath==NULL || nCount<=1 || consumerCount<=4)
    {
        char errorMessage[]="Arguments are not provided properly. Please try again.\n";
        while((write(STDERR_FILENO, errorMessage, sizeof(errorMessage)-1)==-1) && (errno==EINTR));
        exit(EXIT_FAILURE);
    }

    //Creating the semaphores
    int semkey;
    while(((semkey=ftok("/tmp", 'a')))==-1 && errno==EINTR);
    if(semkey==-1)
    {
        perror("Failed to generate key for semaphore");
        exit(EXIT_FAILURE);
    }

    while(((semaphoreSetId=semget(semkey, 2, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)))==-1 && errno==EINTR);
    if(semaphoreSetId==-1)
    {
         perror("Cannot create the semaphore set");
         exit(EXIT_FAILURE);
    }

    //Initializing the semaphores
    int returnVal;
    union semun arg;
    arg.val = 0;
    while(((returnVal=semctl(semaphoreSetId, 0, SETVAL, arg)))==-1 && errno==EINTR);
    if(returnVal==-1)
    {
        perror("Cannot initialize the semaphore 0");
        while((semctl(semaphoreSetId, 0, IPC_RMID))==-1 && errno==EINTR);
        while((semctl(semaphoreSetId, 1, IPC_RMID))==-1 && errno==EINTR);
        exit(EXIT_FAILURE);
    }

    while(((returnVal=semctl(semaphoreSetId, 1, SETVAL, arg)))==-1 && errno==EINTR);
    if(returnVal==-1)
    {
        perror("Cannot initialize the semaphore 1");
        while((semctl(semaphoreSetId, 0, IPC_RMID))==-1 && errno==EINTR);
        while((semctl(semaphoreSetId, 1, IPC_RMID))==-1 && errno==EINTR);
        exit(EXIT_FAILURE);
    }

    //Creating the supplier thread
    pthread_t supplierThread;
    pthread_attr_t supplierAttr;
    while(((returnVal=pthread_attr_init(&supplierAttr)))==-1 && errno==EINTR);
    if(returnVal!=0)
    {
        char errorMessage[]="Failed to initialize pthread attributes\n";
        while((write(STDERR_FILENO, errorMessage, sizeof(errorMessage)-1)==-1) && (errno==EINTR));
        while((semctl(semaphoreSetId, 0, IPC_RMID))==-1 && errno==EINTR);
        while((semctl(semaphoreSetId, 1, IPC_RMID))==-1 && errno==EINTR);
        exit(EXIT_FAILURE);
    }

    while(((returnVal=pthread_attr_setdetachstate(&supplierAttr, PTHREAD_CREATE_DETACHED)))==-1 && errno==EINTR);
    if(returnVal!=0)
    {
        char errorMessage[]="Failed to set detached state.\n";
        while((write(STDERR_FILENO, errorMessage, sizeof(errorMessage)-1)==-1) && (errno==EINTR));
        while((pthread_attr_destroy(&supplierAttr))==-1 && errno==EINTR);
        while((semctl(semaphoreSetId, 0, IPC_RMID))==-1 && errno==EINTR);
        while((semctl(semaphoreSetId, 1, IPC_RMID))==-1 && errno==EINTR);
        exit(EXIT_FAILURE);
    }

    while(((returnVal=pthread_create(&supplierThread, &supplierAttr, supplierFunction, dataFilePath)))==-1 && errno==EINTR);
    if(returnVal!=0)
    {
        char errorMessage[]="Failed to create the supplier thread.\n";
        while((write(STDERR_FILENO, errorMessage, sizeof(errorMessage)-1)==-1) && (errno==EINTR));
        while((pthread_attr_destroy(&supplierAttr))==-1 && errno==EINTR);
        while((semctl(semaphoreSetId, 0, IPC_RMID))==-1 && errno==EINTR);
        while((semctl(semaphoreSetId, 1, IPC_RMID))==-1 && errno==EINTR);
        exit(EXIT_FAILURE);
    }

    while(((returnVal=pthread_attr_destroy(&supplierAttr)))==-1 && errno==EINTR);
    if(returnVal!=0)
    {
        writeOutput("Failed to destroy the pthread attribute\n", STDERR_FILENO);
        sigintFlag=1;
        while((semctl(semaphoreSetId, 0, IPC_RMID))==-1 && errno==EINTR);
        while((semctl(semaphoreSetId, 1, IPC_RMID))==-1 && errno==EINTR);
        pthread_exit(NULL);
    }

    //Creating the consumer threads
    pthread_t consumerThreads[consumerCount];
    int argArray[consumerCount][2];
    for(int i=0; i<consumerCount; ++i)
    {
        argArray[i][0]=nCount;
        argArray[i][1]=i+1;
        while(((returnVal=pthread_create(&consumerThreads[i] , NULL, consumerFunction, argArray[i])))==-1 && errno==EINTR);
        if(returnVal!=0)
        {
            writeOutput("Failed to create consumer thread\n", STDERR_FILENO);
            sigintFlag=1;
            for(int j=0; j<i; ++j)
                while((pthread_join(consumerThreads[j], NULL))==-1 && errno==EINTR);
            while((semctl(semaphoreSetId, 0, IPC_RMID))==-1 && errno==EINTR);
            while((semctl(semaphoreSetId, 1, IPC_RMID))==-1 && errno==EINTR);
            pthread_exit(NULL);
        }
    }

    for(int i=0; i<consumerCount; ++i)
        while((pthread_join(consumerThreads[i], NULL))==-1 && errno==EINTR);

    atexit(removeSems);
    pthread_exit(NULL);
    return 0;
}

void checkArgc(int count)
{
    if(count!=7)
    {
        char errorMessage[]="You need exactly 7 command line arguments (first being auto inputed) to run the program. Please try again.\n";
        while((write(STDERR_FILENO, errorMessage, sizeof(errorMessage)-1)==-1) && (errno==EINTR));
        exit(EXIT_FAILURE);
    }
}

int retrieveArguments(int count, char* arguements[], int* consumer, int* n, char** dataPath)
{
    int option;
    while((option=getopt(count, arguements, "F:N:C:"))!=-1)
        switch(option)
        {
            case 'F':
                *dataPath=optarg;
                break;
            case 'N':
                *n=atoi(optarg);
                break;
            case 'C':
                *consumer=atoi(optarg);
                break;
            default:
                return -1;
        }

    return 0;
}
int openFile(char* filePath, int flags, mode_t mode)
{
    int fd;
    if(mode!=0)
        while((fd=open(filePath, flags, mode))==-1 && errno==EINTR);
    else
        while((fd=open(filePath, flags))==-1 && errno==EINTR);

    return fd;
}

int writeOutput(char* output, int fileFd)
{
    int byteswritten=-1;
    while(((byteswritten=write(fileFd, output, strlen(output)))==-1) && (errno==EINTR));

    return byteswritten;
}

void* consumerFunction(void* args)
{
    //Create semaphore operations
    struct sembuf waitIngredients[2];
    waitIngredients[0].sem_op=-1;
    waitIngredients[1].sem_op=-1;
    waitIngredients[0].sem_flg=0;
    waitIngredients[0].sem_num=0;
    waitIngredients[1].sem_num=1;
    waitIngredients[1].sem_flg=0;

    //Creating semaphore operation to post both
    struct sembuf upBoth[2];
    upBoth[0].sem_num=0;
    upBoth[0].sem_op=1;
    upBoth[0].sem_flg=0;
    upBoth[1].sem_num=1;
    upBoth[1].sem_op=1;
    upBoth[1].sem_flg=0;

    int N=((int*)args)[0];
    int no=((int*)args)[1];
    char output2[1000];
    int returnVal=0;

    unsigned short returnVals[2];
    returnVals[0]=0;
    returnVals[1]=0;
    union semun arg;
    arg.array=returnVals;

    for(int i=0; i<N; ++i)
    {
        if(sigintFlag==1)
        {
            arrangeConsumerOutput3(output2, no);
            writeOutput(output2, STDOUT_FILENO);
            while(((returnVal=semop(semaphoreSetId, upBoth, 2)))==-1 && errno==EINTR);
            return NULL;
        }

        //Before consuming output
        while((semctl(semaphoreSetId, 0, GETALL, arg))==-1 && errno==EINTR);
        arrangeConsumerOutput1(output2, no, i+1, arg.array[0], arg.array[1]);
        if(writeOutput(output2, STDOUT_FILENO)==-1)
            perror("Cannot write the before output of consumer");

        if(sigintFlag==1)
        {
            arrangeConsumerOutput3(output2, no);
            writeOutput(output2, STDOUT_FILENO);
            while(((returnVal=semop(semaphoreSetId, upBoth, 2)))==-1 && errno==EINTR);
            return NULL;
        }

        //Waiting for products then consuming
        while(((returnVal=semop(semaphoreSetId, waitIngredients, 2)))==-1 && errno==EINTR && sigintFlag!=1);
        if(returnVal==-1 && sigintFlag!=1)
        {
            perror("Cannot wait on ingredients");
            arrangeConsumerOutput3(output2, no);
            writeOutput(output2, STDOUT_FILENO);
            sigintFlag=1;
            while(((returnVal=semop(semaphoreSetId, upBoth, 2)))==-1 && errno==EINTR);
            return NULL;
        }

        if(sigintFlag==1)
        {
            arrangeConsumerOutput3(output2, no);
            writeOutput(output2, STDOUT_FILENO);
            while(((returnVal=semop(semaphoreSetId, upBoth, 2)))==-1 && errno==EINTR);
            return NULL;
        }

        //After consuming
        while((semctl(semaphoreSetId, 0, GETALL, arg))==-1 && errno==EINTR);
        arrangeConsumerOutput2(output2, no, i+1, arg.array[0], arg.array[1]);
        if(writeOutput(output2, STDOUT_FILENO)==-1)
            perror("Cannot write the after output of consumer");

        if(sigintFlag==1)
        {
            arrangeConsumerOutput3(output2, no);
            writeOutput(output2, STDOUT_FILENO);
            while(((returnVal=semop(semaphoreSetId, upBoth, 2)))==-1 && errno==EINTR);
            return NULL;
        }
    }

    arrangeConsumerOutput3(output2, no);
    if(writeOutput(output2, STDOUT_FILENO)==-1)
        perror("Cannot write exit output of consumer");

    return NULL;
}

void* supplierFunction(void* args)
{
    //Creating semaphore operation to post sem0
    struct sembuf upS0;
    upS0.sem_num=0;
    upS0.sem_op=1;
    upS0.sem_flg=0;

    //Creating semaphore operation to post sem1
    struct sembuf upS1;
    upS1.sem_num=1;
    upS1.sem_op=1;
    upS1.sem_flg=0;

    //Creating semaphore operation to post both
    struct sembuf upBoth[2];
    upBoth[0].sem_num=0;
    upBoth[0].sem_op=1;
    upBoth[0].sem_flg=0;
    upBoth[1].sem_num=1;
    upBoth[1].sem_op=1;
    upBoth[1].sem_flg=0;

    //Opening the dataFile
    dataFile=openFile((char*)args, O_RDONLY, 0555);
    if(dataFile<0)
    {
        perror("The data file cannot be opened");
        sigintFlag=1;
        while((semop(semaphoreSetId, upBoth, 2))==-1 && errno==EINTR);
        return NULL;
    }

    //Reading from the datafile
    char readChar[1];
    int bytesread;
    int returnVal=0;
    char output[1000];

    unsigned short returnVals[2];
    returnVals[0]=0;
    returnVals[1]=0;
    union semun arg;
    arg.array=returnVals;

    while(true)
    {
        while(((bytesread=read(dataFile, readChar, 1))==-1) && (errno==EINTR));
        if(bytesread<0)
        {
            perror("Cannot read from the file");
            sigintFlag=1;
            while((semop(semaphoreSetId, upBoth, 2))==-1 && errno==EINTR);
            return NULL;
        }
        else if(bytesread==0)
            break;
        else if(readChar[0]=='1')
        {
            if(sigintFlag==1)
            {
                close(dataFile);
                writeOutput("SIGINT received, supplier exiting.\n", STDOUT_FILENO);
                while(((returnVal=semop(semaphoreSetId, upBoth, 2)))==-1 && errno==EINTR);
                return NULL;
            }

            //Before output
            while((semctl(semaphoreSetId, 0, GETALL, arg))==-1 && errno==EINTR);
            arrangeSupplierOutput1(output, '1', arg.array[0], arg.array[1]);
            if(writeOutput(output, STDOUT_FILENO)==-1)
                perror("Cannot write the before output of supplier");

            if(sigintFlag==1)
            {
                close(dataFile);
                writeOutput("SIGINT received, supplier exiting.\n", STDOUT_FILENO);
                while(((returnVal=semop(semaphoreSetId, upBoth, 2)))==-1 && errno==EINTR);
                return NULL;
            }

            //Up sem0
            while((returnVal=semop(semaphoreSetId, &upS0, 1))==-1 && errno==EINTR);
            if(returnVal==-1 && sigintFlag!=1)
            {
                perror("Cannot up sem0");
                sigintFlag=1;
                while((semop(semaphoreSetId, upBoth, 2))==-1 && errno==EINTR);
                return NULL;
            }

            if(sigintFlag==1)
            {
                close(dataFile);
                writeOutput("SIGINT received, supplier exiting.\n", STDOUT_FILENO);
                while(((returnVal=semop(semaphoreSetId, upBoth, 2)))==-1 && errno==EINTR);
                return NULL;
            }

            //After output
            while((semctl(semaphoreSetId, 0, GETALL, arg))==-1 && errno==EINTR);
            arrangeSupplierOutput2(output, '1', arg.array[0], arg.array[1]);
            if(writeOutput(output, STDOUT_FILENO)==-1)
                perror("Cannot write the after output of supplier");

        }
        else if(readChar[0]=='2')
        {
            if(sigintFlag==1)
            {
                close(dataFile);
                writeOutput("SIGINT received, supplier exiting.\n", STDOUT_FILENO);
                while((semop(semaphoreSetId, upBoth, 2))==-1 && errno==EINTR);
                return NULL;
            }

            //Before output
            arrangeSupplierOutput1(output, '2', semctl(semaphoreSetId, 0, GETVAL, returnVal), semctl(semaphoreSetId, 1, GETVAL, returnVal));
            if(writeOutput(output, STDOUT_FILENO)==-1)
                perror("Cannot write the before output of supplier");

            if(sigintFlag==1)
            {
                close(dataFile);
                writeOutput("SIGINT received, supplier exiting.\n", STDOUT_FILENO);
                while((semop(semaphoreSetId, upBoth, 2))==-1 && errno==EINTR);
                return NULL;
            }

            //Up sem1
            while((returnVal=semop(semaphoreSetId, &upS1, 1))==-1 && errno==EINTR);
            if(returnVal==-1)
            {
                perror("Cannot up sem1");
                sigintFlag=1;
                while((semop(semaphoreSetId, upBoth, 2))==-1 && errno==EINTR);
                return NULL;
            }

            if(sigintFlag==1 && sigintFlag!=1)
            {
                close(dataFile);
                writeOutput("SIGINT received, supplier exiting.\n", STDOUT_FILENO);
                while((semop(semaphoreSetId, upBoth, 2))==-1 && errno==EINTR);
                return NULL;
            }

            //After output
            arrangeSupplierOutput2(output, '2', semctl(semaphoreSetId, 0, GETVAL, returnVal), semctl(semaphoreSetId, 1, GETVAL, returnVal));
            if(writeOutput(output, STDOUT_FILENO)==-1)
                perror("Cannot write the after output of supplier");
        }
    }

    close(dataFile);
    return NULL;
}

void arrangeSupplierOutput1(char* output, char readItem, int count1, int count2)
{
    time_t currentTime;
    time(&currentTime);

    struct tm *lt;
    lt=localtime(&currentTime);

    char res[1000];
    char buffer[1000];

    strftime(res, 1000, default_format, lt);
    strcpy(output, res);

    snprintf(buffer, 1000, ": Supplier: read from input a \'%c\'. Current amounts: %dx\'1\', %dx\'2\'.\n", readItem, count1, count2);
    strcat(output, buffer);
}

void arrangeSupplierOutput2(char* output, char deliveredItem, int count1, int count2)
{
    time_t currentTime;
    time(&currentTime);

    struct tm *lt;
    lt=localtime(&currentTime);

    char res[1000];
    char buffer[1000];

    strftime(res, 1000, default_format, lt);
    strcpy(output, res);

    snprintf(buffer, 1000, ": Supplier: delivered a \'%c\'. Post-delivery amounts: %dx\'1\', %dx\'2\'.\n", deliveredItem, count1, count2);
    strcat(output, buffer);
}

void arrangeConsumerOutput1(char* output, int no, int iterationNo, int count1, int count2)
{
    time_t currentTime;
    time(&currentTime);

    struct tm *lt;
    lt=localtime(&currentTime);

    char res[1000];
    char buffer[1000];

    strftime(res, 1000, default_format, lt);
    strcpy(output, res);

    snprintf(buffer, 1000, ": Consumer-%d at iteration %d (waiting). Current amounts: %dx\'1\', %dx\'2\'.\n", no, iterationNo, count1, count2);
    strcat(output, buffer);
}

void arrangeConsumerOutput2(char* output, int no, int iterationNo, int count1, int count2)
{
    time_t currentTime;
    time(&currentTime);

    struct tm *lt;
    lt=localtime(&currentTime);

    char res[1000];
    char buffer[1000];

    strftime(res, 1000, default_format, lt);
    strcpy(output, res);

    snprintf(buffer, 1000, ": Consumer-%d at iteration %d (consumed). Post-consumption amounts: %dx\'1\', %dx\'2\'.\n", no, iterationNo, count1, count2);
    strcat(output, buffer);
}

void arrangeConsumerOutput3(char* output, int no)
{
    time_t currentTime;
    time(&currentTime);

    struct tm *lt;
    lt=localtime(&currentTime);

    char res[1000];
    char buffer[1000];

    strftime(res, 1000, default_format, lt);
    strcpy(output, res);

    snprintf(buffer, 1000, ": Consumer-%d has left\n", no);
    strcat(output, buffer);
}

void sigintHandler(int signum)
{
    sigintFlag=1;
}

void removeSems()
{
    while((semctl(semaphoreSetId, 0, IPC_RMID))==-1 && errno==EINTR);
    while((semctl(semaphoreSetId, 1, IPC_RMID))==-1 && errno==EINTR);
}
