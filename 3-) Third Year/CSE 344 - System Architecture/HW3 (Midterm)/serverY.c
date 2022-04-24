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
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <semaphore.h>
#include <sys/mman.h>

#define BD_CHDIR           00
#define BD_CLOSE_FILES     02
#define BD_REOPEN_STD_FDS  04
#define BD_UMASK          010
#define BD_MAX_CLOSE  8192

struct childStatus{
    int childID;
    int pipoFileDes[2];
    bool free;
};

struct childZStatus{
    int childID;
    bool free;
};

struct requestQueue{
    int currentIndex;
    int totalRequests;
    char requests[10][2048];
};

sem_t *serverYSem=NULL;
sem_t* resultSharedMemSem=NULL;
sem_t* requestsSem=NULL;
sem_t* emptySem=NULL;
sem_t* fullSem=NULL;
static const char default_format[] = "%b %d %Y %Z %H %M %S";
int poolBusy=0;
int workerCount=0;
struct childStatus* childrenIDs;
struct childZStatus* zWorkers;
int workerZCount=0;
int serverZID=-1;
int sigintFlag=0;
int opened=0;
char* fifoFileName=NULL;
int fifoFakeWriter;

void sigusr1Handler(int signo, siginfo_t *si, void *data);
void sigintHandler(int signum);

void checkArgc(int count);
int retrieveArguments(int count, char* arguements[], char** fifoPath, char** logPath, int* pool1, int* pool2, int* sleep);
int becomeDaemon(int flags);
void upSem();
int openFile(char* filePath, int flags, mode_t mode);
int createFifo(char* fifoPath);
void createMessage(char* startMessage, char* message);
void writeErrorMessage(char* startMessage, int logFd);
void arrangeOutput1(char* output, char* fileName, int poolSize, int sleepTime);
void freePointer(void* pointer);
char* addDigit(char newDigit, char** oldNumber, int* oldNumberSize);
bool isInvertible(int** matrix, int size);
void arrangeWorkerOutput1(char* output, int worderPid, int clientPid, int matrixSize, bool isZ, int busy, int count);
void arrangeWorkerOutput2(char* output, int clientPid, bool result, bool isZ);
long long int determinant(int** matrix, int size);
void terminateChildren();
void terminateZChildren();
void arrangeForwardOutput(char* output, int clientPID, int clientMatrixSize);
void arrangeServerZOutput1(char* output, char* logFile, int sleepTime, int workerCount);
void arrangeSigintOutput(char* output, int total, int forwarded, int invertibleCount, bool isZ);

int main(int argc, char* argv[])
{
    setenv("TZ", "Europe/Istanbul", 1);
    tzset();

    //Checking if there is any other instance of this already
    while(((serverYSem=sem_open("/SeverYInit", O_CREAT, 0644, 1))==NULL) && errno==EINTR);
    if(serverYSem==SEM_FAILED)
    {
        perror("Cannot open semaphore");
        exit(EXIT_FAILURE);
    }

    int semVal;
    int returnVal;
    while(((returnVal=sem_getvalue(serverYSem, &semVal))==-1) && errno==EINTR);
    if(returnVal==-1)
    {
        perror("Cannot receive the semaphore value");
        sem_close(serverYSem);
        exit(EXIT_FAILURE);
    }

    if(semVal==0)
    {
        char errorMessage[]="There is already an active serverY. Cannot initiate a second one.\n";
        while((write(STDERR_FILENO, errorMessage, sizeof(errorMessage)-1)==-1) && (errno==EINTR));
        sem_close(serverYSem);
        exit(EXIT_FAILURE);
    }

    //Checking command line arguments
    checkArgc(argc);

    //Retrieving the command line arguments & checking validity
    char* logFileName=NULL;
    int poolSize=-1;
    int poolSize2=-1;
    int sleepTime=-1;
    if(retrieveArguments(argc, argv, &fifoFileName, &logFileName, &poolSize, &poolSize2, &sleepTime)==-1)
    {
        char errorMessage[]="There is an invalid command line argument. You can only use \"-s\", \"-o\", \"-p\", \"-r\" and \"-t\". Please try again.\n";
        while((write(STDERR_FILENO, errorMessage, sizeof(errorMessage)-1)==-1) && (errno==EINTR));
        sem_close(serverYSem);
        exit(EXIT_FAILURE);
    }

    //Checking argument validity
    if(fifoFileName==NULL || logFileName==NULL || sleepTime==-1 || poolSize<2 || poolSize2<2)
    {
        char errorMessage[]="Arguments are not provided properly. Please try again.\n";
        while((write(STDERR_FILENO, errorMessage, sizeof(errorMessage)-1)==-1) && (errno==EINTR));
        sem_close(serverYSem);
        exit(EXIT_FAILURE);
    }

    //Waiting on the server semaphore not to let other servers being initiated.
    while(((returnVal=sem_wait(serverYSem))==-1) && errno==EINTR);
    if(returnVal==-1)
    {
        perror("Cannot wait on semaphore");
        sem_close(serverYSem);
        exit(EXIT_FAILURE);
    }

    //Transforming into a daemon process
    if(becomeDaemon(0)==-1)
    {
        char errorMessage[]="Failed to become a daemon. Please try again.\n";
        while((write(STDERR_FILENO, errorMessage, sizeof(errorMessage)-1)==-1) && (errno==EINTR));
        sem_close(serverYSem);
        exit(EXIT_FAILURE);
    }

    atexit(upSem);
    //Opening the log file
    int logFile=openFile(logFileName, O_WRONLY | O_CREAT | O_TRUNC, 0777);
    if(logFile==-1)
        exit(EXIT_FAILURE);

    //SIGINT handler
    struct sigaction sigintAct={0};
    sigemptyset(&sigintAct.sa_mask);
    sigaddset(&sigintAct.sa_mask, SIGRTMIN);
    sigintAct.sa_handler=sigintHandler;
    while(((returnVal=sigaction(SIGINT, &sigintAct, NULL))==-1) && errno==EINTR);
    if(returnVal==-1)
    {
        writeErrorMessage("Cannot assign signal handler.\n", logFile);
        close(logFile);
        exit(EXIT_FAILURE);
    }

    //Init output
    char serverYInitOutput[300];
    arrangeOutput1(serverYInitOutput, logFileName, poolSize, sleepTime);
    int byteswritten;
    while(((byteswritten=write(logFile, serverYInitOutput, strlen(serverYInitOutput)))==-1) && (errno==EINTR));
    if(byteswritten==-1)
    {
        writeErrorMessage("Write failed.\n", logFile);
        close(logFile);
        exit(EXIT_FAILURE);
    }

    //Creating the server fifo if it does not exist
    if(createFifo(fifoFileName)==-1)
    {
        writeErrorMessage("FIFO cannot be created.\n", logFile);
        close(logFile);
        exit(EXIT_FAILURE);
    }

    //Opening the server fifo
    int fifoFile=openFile(fifoFileName, O_RDONLY, 0);
    if(fifoFile==-1)
    {
        writeErrorMessage("Cannot open server FIFO.\n", logFile);
        close(logFile);
        unlink(fifoFileName);
        exit(EXIT_FAILURE);
    }

    opened=1;
    //Opening the server fifo as a writer too not to never have 0 writers (for blocking purposes)
    fifoFakeWriter=openFile(fifoFileName, O_WRONLY, 0);
    if(fifoFakeWriter==-1)
    {
        writeErrorMessage("Cannot open server FIFO.\n", logFile);
        close(logFile);
        close(fifoFile);
        unlink(fifoFileName);
        exit(EXIT_FAILURE);
    }

    if(sigintFlag==1)
    {
        writeErrorMessage("SIGINT received. Exiting server Y. No serverZ or workers were initialized yet.\n", logFile);
        close(logFile);
        close(fifoFile);
        close(fifoFakeWriter);
        unlink(fifoFileName);
        exit(EXIT_FAILURE);
    }

    //Opening a shared memory for invertible and non invertible matrices
    int resultMemFd;
    while(((resultMemFd=shm_open("/ResultMemory", O_CREAT | O_RDWR, 0777))==-1) && errno==EINTR);
    if(resultMemFd==-1)
    {
        writeErrorMessage("Cannot create shared memory for results\n", logFile);
        close(logFile);
        close(fifoFile);
        close(fifoFakeWriter);
        unlink(fifoFileName);
        exit(EXIT_FAILURE);
    }

    while(((returnVal=ftruncate(resultMemFd, sizeof(int)*2))==-1) && errno==EINTR);
    if(returnVal==-1)
    {
        writeErrorMessage("Cannot adjust size of shared memory\n", logFile);
        close(resultMemFd);
        close(logFile);
        close(fifoFile);
        close(fifoFakeWriter);
        unlink(fifoFileName);
        shm_unlink("/ResultMemory");
        exit(EXIT_FAILURE);
    }

    int* resultMemAdd;
    while(((resultMemAdd=mmap(NULL, sizeof(int)*2, PROT_READ | PROT_WRITE, MAP_SHARED, resultMemFd, 0))==MAP_FAILED) && errno==EINTR);
    if(resultMemAdd==MAP_FAILED)
    {
        writeErrorMessage("Mapping of result shared memory failed\n", logFile);
        close(resultMemFd);
        close(logFile);
        close(fifoFile);
        close(fifoFakeWriter);
        unlink(fifoFileName);
        shm_unlink("/ResultMemory");
        exit(EXIT_FAILURE);
    }

    close(resultMemFd);
    int start=0;
    memcpy(resultMemAdd, &start, sizeof(int));
    memcpy(resultMemAdd+sizeof(int), &start, sizeof(int));
    memcpy(resultMemAdd+(sizeof(int)*2), &start, sizeof(int));

    while(((resultSharedMemSem=sem_open("/resultSharedMemory", O_CREAT, 0777, 1))==SEM_FAILED) && errno==EINTR);
    if(resultSharedMemSem==SEM_FAILED)
    {
        writeErrorMessage("Cannot open resultSharedMemory semaphore\n.", logFile);
        close(logFile);
        close(fifoFile);
        close(fifoFakeWriter);
        unlink(fifoFileName);
        shm_unlink("/ResultMemory");
        munmap("/ResultMemory", sizeof(int)*2);
        exit(EXIT_FAILURE);
    }

    //Initiate serverZ
    char serverZInitOutput[100];
    createMessage("Instantiated server Z\n", serverZInitOutput);
    while(((byteswritten=write(logFile, serverZInitOutput, strlen(serverZInitOutput)))==-1) && (errno==EINTR));
    if(byteswritten==-1)
    {
        struct flock lock;
        lock.l_type=F_UNLCK;
        while(((fcntl(logFile, F_SETLKW, &lock))==-1) && errno==EINTR);
        writeErrorMessage("Write failed.\n", logFile);
        close(logFile);
        close(fifoFile);
        close(fifoFakeWriter);
        unlink(fifoFileName);
        shm_unlink("/ResultMemory");
        munmap("/ResultMemory", sizeof(int)*2);
        exit(EXIT_FAILURE);
    }

    int serverZPipe[2];
    while(((returnVal=pipe(serverZPipe))==-1) && errno==EINTR);
    if(returnVal==-1)
    {
        writeErrorMessage("Cannot create pipe for server z.\n", logFile);
        close(logFile);
        close(fifoFile);
        close(fifoFakeWriter);
        unlink(fifoFileName);
        shm_unlink("/ResultMemory");
        munmap("/ResultMemory", sizeof(int)*2);
        exit(EXIT_FAILURE);
    }

    serverZID=fork();
    if(serverZID==-1)
    {
        writeErrorMessage("Fork failed to create server z.\n", logFile);
        close(logFile);
        close(fifoFile);
        close(fifoFakeWriter);
        close(serverZPipe[0]);
        close(serverZPipe[1]);
        unlink(fifoFileName);
        shm_unlink("/ResultMemory");
        munmap("/ResultMemory", sizeof(int)*2);
        exit(EXIT_FAILURE);
    }
    else if(serverZID>0)
        close(serverZPipe[0]);
    else if(serverZID==0)
    {
        close(fifoFakeWriter);
        close(serverZPipe[1]);
        close(fifoFile);

        int totalRequests=0;
        //Locking the log file for writing
        struct flock lock;
        memset(&lock, 0, sizeof(lock));
        lock.l_type=F_WRLCK;
        while(((fcntl(logFile, F_SETLKW, &lock))==-1) && errno==EINTR);

        //Start output
        char serverZStartOutput[300];
        arrangeServerZOutput1(serverZStartOutput, logFileName, sleepTime, poolSize2);
        while(((byteswritten=write(logFile, serverZStartOutput, strlen(serverZStartOutput)))==-1) && (errno==EINTR));
        if(byteswritten<0)
        {
            writeErrorMessage("Failed to write server z start output\n", logFile);
            close(serverZPipe[0]);
            close(logFile);
            shm_unlink("/ResultMemory");
            munmap("/ResultMemory", sizeof(int)*2);
            exit(EXIT_FAILURE);
        }

        //Unlocking
        lock.l_type=F_UNLCK;
        while(((fcntl(logFile, F_SETLKW, &lock))==-1) && errno==EINTR);

        //Initialize the workers
        zWorkers=(struct childZStatus*)malloc(sizeof(struct childZStatus)*poolSize2);
        if(zWorkers==NULL)
        {
            writeErrorMessage("Cannot allocate memory for zWorkers\n", logFile);
            close(serverZPipe[0]);
            close(logFile);
            shm_unlink("/ResultMemory");
            munmap("/ResultMemory", sizeof(int)*2);
            exit(EXIT_FAILURE);
        }

        //Opening a shared memory for the requests
        int requestsMemFd;
        while(((requestsMemFd=shm_open("/Requests", O_CREAT | O_RDWR, 0777))==-1) && errno==EINTR);
        if(requestsMemFd==-1)
        {
            writeErrorMessage("Cannot create shared memory for requests\n", logFile);
            close(serverZPipe[0]);
            close(logFile);
            free(zWorkers);
            shm_unlink("/ResultMemory");
            munmap("/ResultMemory", sizeof(int)*2);
            exit(EXIT_FAILURE);
        }

        while(((returnVal=ftruncate(requestsMemFd, sizeof(struct requestQueue)))==-1) && errno==EINTR);
        if(returnVal==-1)
        {
            writeErrorMessage("Cannot adjust size of shared memory\n", logFile);
            close(serverZPipe[0]);
            close(logFile);
            shm_unlink("/ResultMemory");
            munmap("/ResultMemory", sizeof(int)*2);
            shm_unlink("/Requests");
            free(zWorkers);
            close(requestsMemFd);
            exit(EXIT_FAILURE);
        }

        struct requestQueue* requestMemAdd;
        while(((requestMemAdd=mmap(NULL, sizeof(struct requestQueue), PROT_READ | PROT_WRITE, MAP_SHARED, requestsMemFd, 0))==MAP_FAILED) && errno==EINTR);
        if(requestMemAdd==MAP_FAILED)
        {
            writeErrorMessage("Mapping of requests shared memory failed\n", logFile);
            close(serverZPipe[0]);
            close(logFile);
            free(zWorkers);
            shm_unlink("/ResultMemory");
            munmap("/ResultMemory", sizeof(int)*2);
            shm_unlink("/Requests");
            close(requestsMemFd);
            exit(EXIT_FAILURE);
        }

        close(requestsMemFd);
        (*requestMemAdd).currentIndex=0;
        (*requestMemAdd).totalRequests=0;

        while(((requestsSem=sem_open("/requestsSem", O_CREAT, 0777, 1))==NULL) && errno==EINTR);
        if(requestsSem==SEM_FAILED)
        {
            writeErrorMessage("Cannot open requestsSem semaphore\n.", logFile);
            close(serverZPipe[0]);
            close(logFile);
            free(zWorkers);
            shm_unlink("/ResultMemory");
            munmap("/ResultMemory", sizeof(int)*2);
            shm_unlink("/Requests");
            munmap("/Requests", sizeof(struct requestQueue));
            exit(EXIT_FAILURE);
        }

        while(((emptySem=sem_open("/emptySem", O_CREAT, 0777, 0))==NULL) && errno==EINTR);
        if(emptySem==SEM_FAILED)
        {
            writeErrorMessage("Cannot open emptySem semaphore\n.", logFile);
            close(serverZPipe[0]);
            close(logFile);
            free(zWorkers);
            shm_unlink("/ResultMemory");
            munmap("/ResultMemory", sizeof(int)*2);
            shm_unlink("/Requests");
            munmap("/Requests", sizeof(struct requestQueue));
            exit(EXIT_FAILURE);
        }

        while(((fullSem=sem_open("/fullSem", O_CREAT, 0777, 10))==NULL) && errno==EINTR);
        if(fullSem==SEM_FAILED)
        {
            writeErrorMessage("Cannot open fullSem semaphore\n.", logFile);
            close(serverZPipe[0]);
            close(logFile);
            free(zWorkers);
            shm_unlink("/ResultMemory");
            munmap("/ResultMemory", sizeof(int)*2);
            shm_unlink("/Requests");
            munmap("/Requests", sizeof(struct requestQueue));
            exit(EXIT_FAILURE);
        }

        if(sigintFlag==1)
        {
            writeErrorMessage("Z: SIGINT received, exiting server Z. No requests were received.\n", logFile);
            close(serverZPipe[0]);
            close(logFile);
            free(zWorkers);
            shm_unlink("/ResultMemory");
            munmap("/ResultMemory", sizeof(int)*2);
            shm_unlink("/Requests");
            munmap("/Requests", sizeof(struct requestQueue));
            return 0;
        }

        for(int i=0; i<poolSize2; ++i)
        {
            workerZCount++;
            int childID=fork();
            if(childID==-1)
            {
                writeErrorMessage("Fork failed in serverZ\n", logFile);
                workerZCount--;
                terminateZChildren();
                close(serverZPipe[0]);
                close(logFile);
                free(zWorkers);
                shm_unlink("/ResultMemory");
                munmap("/ResultMemory", sizeof(int)*2);
                shm_unlink("/Requests");
                munmap("/Requests", sizeof(struct requestQueue));
                exit(EXIT_FAILURE);
            }
            else if(childID>0)
            {
                zWorkers[i].childID=childID;
                zWorkers[i].free=true;
            }
            else if(childID==0) //Z Worker Processing here
            {
                close(serverZPipe[0]);
                while(true)
                {
                    while(((returnVal=sem_wait(emptySem))==-1) && errno==EINTR);
                    if(returnVal==-1)
                    {
                        writeErrorMessage("Cannot wait on semaphore.\n", logFile);
                        close(logFile);
                        free(zWorkers);
                        shm_unlink("/ResultMemory");
                        munmap("/ResultMemory", sizeof(int)*2);
                        shm_unlink("/Requests");
                        munmap("/Requests", sizeof(struct requestQueue));
                        exit(EXIT_FAILURE);
                    }
                    while(((returnVal=sem_wait(requestsSem))==-1) && errno==EINTR);
                    if(returnVal==-1)
                    {
                        writeErrorMessage("Cannot wait on semaphore.\n", logFile);
                        close(logFile);
                        free(zWorkers);
                        sem_post(emptySem);
                        shm_unlink("/ResultMemory");
                        munmap("/ResultMemory", sizeof(int)*2);
                        shm_unlink("/Requests");
                        munmap("/Requests", sizeof(struct requestQueue));
                        exit(EXIT_FAILURE);
                    }

                    if(sigintFlag==1)
                    {
                        writeErrorMessage("Z: Z worker received SIGINT. Terminating the worker.\n", logFile);
                        sem_post(emptySem);
                        sem_post(requestsSem);
                        close(logFile);
                        free(zWorkers);
                        shm_unlink("/ResultMemory");
                        munmap("/ResultMemory", sizeof(int)*2);
                        shm_unlink("/Requests");
                        munmap("/Requests", sizeof(struct requestQueue));
                        return 0;
                    }

                    char* readRequest=(*requestMemAdd).requests[(*requestMemAdd).currentIndex];
                    (*requestMemAdd).currentIndex++;

                    if((*requestMemAdd).currentIndex==10)
                        (*requestMemAdd).currentIndex=0;

                    while(((returnVal=sem_post(requestsSem))==-1) && errno==EINTR);
                    if(returnVal==-1)
                    {
                        writeErrorMessage("Cannot post on semaphore.\n", logFile);
                        close(logFile);
                        sem_post(requestsSem);
                        sem_post(fullSem);
                        free(zWorkers);
                        shm_unlink("/ResultMemory");
                        munmap("/ResultMemory", sizeof(int)*2);
                        shm_unlink("/Requests");
                        munmap("/Requests", sizeof(struct requestQueue));
                        exit(EXIT_FAILURE);
                    }
                    while(((returnVal=sem_post(fullSem))==-1) && errno==EINTR);
                    if(returnVal==-1)
                    {
                        writeErrorMessage("Cannot post on semaphore.\n", logFile);
                        close(logFile);
                        free(zWorkers);
                        sem_post(fullSem);
                        shm_unlink("/ResultMemory");
                        munmap("/ResultMemory", sizeof(int)*2);
                        shm_unlink("/Requests");
                        munmap("/Requests", sizeof(struct requestQueue));
                        exit(EXIT_FAILURE);
                    }
                    (*(resultMemAdd+(sizeof(int)*2)))++;

                    //Processing the request
                    int clientPID=-1;
                    int clientMatrixSize=-1;
                    char* readNumber=NULL;
                    int** matrix=NULL;
                    int rowCount=0;
                    int columnCount=0;
                    int currentDigit=0;
                    for(int i=0; i<strlen(readRequest); ++i)
                    {
                        char readChar[1];
                        readChar[0]=readRequest[i];
                        if(readChar[0]==' ' && clientPID==-1)
                        {
                            clientPID=atoi(readNumber);
                            free(readNumber);
                            readNumber=NULL;
                            currentDigit=0;
                        }
                        else if(readChar[0]==' ' && clientMatrixSize==-1)
                        {
                            clientMatrixSize=atoi(readNumber);
                            free(readNumber);
                            readNumber=NULL;

                            matrix=(int**)malloc(clientMatrixSize*sizeof(int*));
                            for(int i=0; i<clientMatrixSize; ++i)
                                matrix[i]=(int*)malloc(clientMatrixSize*sizeof(int));
                            currentDigit=0;
                        }
                        else if(readChar[0]=='\n' || readChar[0]==' ')
                        {
                            matrix[rowCount][columnCount]=atoi(readNumber);
                            free(readNumber);
                            readNumber=NULL;

                            columnCount++;
                            if(columnCount==clientMatrixSize)
                            {
                                columnCount=0;
                                rowCount++;
                            }
                            currentDigit=0;

                            if(readChar[0]=='\n')
                                break;
                        }
                        else if(readChar[0]!='\r')
                        {
                            readNumber=addDigit(*readChar, &readNumber, &currentDigit);
                            if(readNumber==NULL)
                            {
                                writeErrorMessage("Cannot allocate memory for readNumber in worker.\n", logFile);
                                freePointer(readNumber);
                                if(matrix!=NULL)
                                {
                                    for(int i=0; i<clientMatrixSize; ++i)
                                        free(matrix[i]);
                                    free(matrix);
                                }
                                exit(EXIT_FAILURE);
                                close(logFile);
                                shm_unlink("/ResultMemory");
                                free(zWorkers);
                                munmap("/ResultMemory", sizeof(int)*2);
                                shm_unlink("/Requests");
                                munmap("/Requests", sizeof(struct requestQueue));
                                exit(EXIT_FAILURE);
                            }
                        }
                    }

                    //Locking the log file for writing
                    memset(&lock, 0, sizeof(lock));
                    lock.l_type=F_WRLCK;
                    while(((fcntl(logFile, F_SETLKW, &lock))==-1) && errno==EINTR);

                    //Writing second output
                    char workerOutput1[300];
                    arrangeWorkerOutput1(workerOutput1, getpid(), clientPID, clientMatrixSize, true, *(resultMemAdd+(sizeof(int)*2)), poolSize2);
                    while(((byteswritten=write(logFile, workerOutput1, strlen(workerOutput1)))==-1) && (errno==EINTR));
                    if(byteswritten==-1)
                    {
                        lock.l_type=F_UNLCK;
                        while(((fcntl(logFile, F_SETLKW, &lock))==-1) && errno==EINTR);
                        writeErrorMessage("Second worker output write failed.\n", logFile);
                        if(matrix!=NULL)
                        {
                            for(int i=0; i<clientMatrixSize; ++i)
                                free(matrix[i]);
                            free(matrix);
                        }
                        close(logFile);
                        shm_unlink("/ResultMemory");
                        free(zWorkers);
                        munmap("/ResultMemory", sizeof(int)*2);
                        shm_unlink("/Requests");
                        munmap("/Requests", sizeof(struct requestQueue));
                        exit(EXIT_FAILURE);
                    }

                    lock.l_type=F_UNLCK;
                    while(((fcntl(logFile, F_SETLKW, &lock))==-1) && errno==EINTR);

                    if(matrix!=NULL)
                    {
                        bool result=isInvertible(matrix, clientMatrixSize);

                        if(result==true)
                        {
                            while(((returnVal=sem_wait(resultSharedMemSem))==-1) && errno==EINTR);
                            if(returnVal==-1)
                            {
                                writeErrorMessage("Cannot wait on resultSharedMem semaphore\n", logFile);
                                sem_post(resultSharedMemSem);
                                if(matrix!=NULL)
                                {
                                    for(int i=0; i<clientMatrixSize; ++i)
                                        free(matrix[i]);
                                    free(matrix);
                                }
                                close(logFile);
                                shm_unlink("/ResultMemory");
                                free(zWorkers);
                                munmap("/ResultMemory", sizeof(int)*2);
                                shm_unlink("/Requests");
                                munmap("/Requests", sizeof(struct requestQueue));
                                exit(EXIT_FAILURE);
                            }

                            (*resultMemAdd)++;
                            (*(resultMemAdd+sizeof(int)))++;

                            while(((returnVal=sem_post(resultSharedMemSem))==-1) && errno==EINTR);
                            if(returnVal==-1)
                            {
                                writeErrorMessage("Cannot post on resultSharedMem semaphore\n", logFile);
                                sem_post(resultSharedMemSem);
                                if(matrix!=NULL)
                                {
                                    for(int i=0; i<clientMatrixSize; ++i)
                                        free(matrix[i]);
                                    free(matrix);
                                }
                                close(logFile);
                                free(zWorkers);
                                shm_unlink("/ResultMemory");
                                munmap("/ResultMemory", sizeof(int)*2);
                                shm_unlink("/Requests");
                                munmap("/Requests", sizeof(struct requestQueue));
                                exit(EXIT_FAILURE);
                            }
                        }

                        //Locking the log file for writing
                        memset(&lock, 0, sizeof(lock));
                        lock.l_type=F_WRLCK;
                        while(((fcntl(logFile, F_SETLKW, &lock))==-1) && errno==EINTR);

                        //Writing second output
                        char workerOutput[300];
                        arrangeWorkerOutput2(workerOutput, clientPID, result, true);
                        while(((byteswritten=write(logFile, workerOutput, strlen(workerOutput)))==-1) && (errno==EINTR));
                        if(byteswritten==-1)
                        {
                            lock.l_type=F_UNLCK;
                            while(((fcntl(logFile, F_SETLKW, &lock))==-1) && errno==EINTR);
                            writeErrorMessage("Second worker output write failed.\n", logFile);
                            if(matrix!=NULL)
                            {
                                for(int i=0; i<clientMatrixSize; ++i)
                                    free(matrix[i]);
                                free(matrix);
                            }
                            close(logFile);
                            shm_unlink("/ResultMemory");
                            free(zWorkers);
                            munmap("/ResultMemory", sizeof(int)*2);
                            shm_unlink("/Requests");
                            munmap("/Requests", sizeof(struct requestQueue));
                            exit(EXIT_FAILURE);
                        }

                        lock.l_type=F_UNLCK;
                        while(((fcntl(logFile, F_SETLKW, &lock))==-1) && errno==EINTR);

                        //Sending the response
                        char clientFifoPath[100];
                        char buffer[100];

                        sprintf(buffer, "/tmp/%d", clientPID);
                        strcpy(clientFifoPath, buffer);
                        strcat(clientFifoPath, "FIFO.fifo");

                        int clientFifoFile=openFile(clientFifoPath, O_WRONLY, 0);
                        if(clientFifoFile==-1)
                        {
                            writeErrorMessage("Opening client fifo file failed.\n", logFile);
                            if(matrix!=NULL)
                            {
                                for(int i=0; i<clientMatrixSize; ++i)
                                    free(matrix[i]);
                                free(matrix);
                            }
                            close(logFile);
                            shm_unlink("/ResultMemory");
                            free(zWorkers);
                            munmap("/ResultMemory", sizeof(int)*2);
                            shm_unlink("/Requests");
                            munmap("/Requests", sizeof(struct requestQueue));
                            exit(EXIT_FAILURE);
                        }

                        char response[5];
                        if(result==true)
                            strcpy(response, "Yes");
                        else
                            strcpy(response, "No");

                        sleep(sleepTime);

                        while(((byteswritten=write(clientFifoFile, response, strlen(response)))==-1) && errno==EINTR);
                        if(byteswritten<0)
                        {
                            writeErrorMessage("Write response to client fifo file failed.\n", logFile);
                            if(matrix!=NULL)
                            {
                                for(int i=0; i<clientMatrixSize; ++i)
                                    free(matrix[i]);
                                free(matrix);
                            }
                            close(logFile);
                            close(clientFifoFile);
                            shm_unlink("/ResultMemory");
                            free(zWorkers);
                            munmap("/ResultMemory", sizeof(int)*2);
                            shm_unlink("/Requests");
                            munmap("/Requests", sizeof(struct requestQueue));
                            exit(EXIT_FAILURE);
                        }

                        close(clientFifoFile);
                        if(matrix!=NULL)
                        {
                            for(int i=0; i<clientMatrixSize; ++i)
                                free(matrix[i]);
                            free(matrix);
                        }
                    }
                    (*(resultMemAdd+(sizeof(int)*2)))--;
                }

                close(logFile);
                free(zWorkers);
                shm_unlink("/ResultMemory");
                munmap("/ResultMemory", sizeof(int)*2);
                shm_unlink("/Requests");
                munmap("/Requests", sizeof(struct requestQueue));
                exit(EXIT_SUCCESS);
            }
        }

        //Start real processing of serverZ
        while(true)
        {
            //Read from pipe
            char* readRequest=NULL;
            char* readNumber=NULL;
            int clientPID=-1;
            int clientMatrixSize=-1;
            int size=0;
            int size2=0;
            while(true)
            {
                char readChar[1];
                int bytesread;
                while(((bytesread=read(serverZPipe[0], readChar, 1))==-1) && errno==EINTR);
                if(bytesread<0)
                {
                    writeErrorMessage("Read from server z pipe failed.\n", logFile);
                    terminateZChildren();
                    close(serverZPipe[0]);
                    free(zWorkers);
                    close(logFile);
                    freePointer(readRequest);
                    freePointer(readNumber);
                    shm_unlink("/ResultMemory");
                    munmap("/ResultMemory", sizeof(int)*2);
                    shm_unlink("/Requests");
                    munmap("/Requests", sizeof(struct requestQueue));
                    exit(EXIT_FAILURE);
                }
                else if(bytesread==0)
                    break;
                else
                {
                    if((clientPID==-1 || clientMatrixSize==-1) && readChar[0]!=' ' && readChar[0]!='\r')
                        readNumber=addDigit(readChar[0], &readNumber, &size2);
                    readRequest=addDigit(readChar[0], &readRequest, &size);
                    if(readRequest==NULL || ((readNumber==NULL) && ((clientPID==-1) || (clientMatrixSize==-1)) ) )
                    {
                        writeErrorMessage("Cannot allocate memory for request in serverY.\n", logFile);
                        terminateZChildren();
                        close(serverZPipe[0]);
                        free(zWorkers);
                        close(logFile);
                        freePointer(readRequest);
                        freePointer(readNumber);
                        shm_unlink("/ResultMemory");
                        munmap("/ResultMemory", sizeof(int)*2);
                        shm_unlink("/Requests");
                        munmap("/Requests", sizeof(struct requestQueue));
                        exit(EXIT_FAILURE);
                    }
                    if(readChar[0]=='\n')
                        break;
                    else if(readChar[0]==' ' && clientPID==-1)
                    {
                        clientPID=atoi(readNumber);
                        free(readNumber);
                        readNumber=NULL;
                        size2=0;
                    }
                    else if(readChar[0]==' ' && clientMatrixSize==-1)
                    {
                        clientMatrixSize=atoi(readNumber);
                        free(readNumber);
                        readNumber=NULL;
                        size2=0;
                    }
                }
            }
            free(readNumber);

            //Put the request in a queue
            while(((returnVal=sem_wait(fullSem))==-1) && errno==EINTR);
            if(returnVal==-1)
            {
                writeErrorMessage("Cannot wait on semaphore.\n", logFile);
                terminateZChildren();
                close(serverZPipe[0]);
                close(logFile);
                free(zWorkers);
                shm_unlink("/ResultMemory");
                munmap("/ResultMemory", sizeof(int)*2);
                shm_unlink("/Requests");
                munmap("/Requests", sizeof(struct requestQueue));
                exit(EXIT_FAILURE);
            }
            while(((returnVal=sem_wait(requestsSem))==-1) && errno==EINTR);
            if(returnVal==-1)
            {
                writeErrorMessage("Cannot wait on semaphore.\n", logFile);
                terminateZChildren();
                close(serverZPipe[0]);
                close(logFile);
                free(zWorkers);
                shm_unlink("/ResultMemory");
                munmap("/ResultMemory", sizeof(int)*2);
                shm_unlink("/Requests");
                munmap("/Requests", sizeof(struct requestQueue));
                exit(EXIT_FAILURE);
            }

            if(sigintFlag==1)
            {
                char buffer2[400];
                arrangeSigintOutput(buffer2, totalRequests, 0, *(resultMemAdd+sizeof(int)), true);
                writeErrorMessage(buffer2, logFile);
                terminateZChildren();
                close(serverZPipe[0]);
                close(logFile);
                free(zWorkers);
                shm_unlink("/ResultMemory");
                munmap("/ResultMemory", sizeof(int)*2);
                shm_unlink("/Requests");
                munmap("/Requests", sizeof(struct requestQueue));
                return 0;
            }

            strcpy((*requestMemAdd).requests[(*requestMemAdd).totalRequests], readRequest);
            (*requestMemAdd).totalRequests++;
            totalRequests++;
            if((*requestMemAdd).totalRequests==10)
                (*requestMemAdd).totalRequests=0;

            while(((returnVal=sem_post(requestsSem))==-1) && errno==EINTR);
            if(returnVal==-1)
            {
                writeErrorMessage("Cannot wait on semaphore.\n", logFile);
                terminateZChildren();
                close(serverZPipe[0]);
                close(logFile);
                free(zWorkers);
                shm_unlink("/ResultMemory");
                munmap("/ResultMemory", sizeof(int)*2);
                shm_unlink("/Requests");
                munmap("/Requests", sizeof(struct requestQueue));
                exit(EXIT_FAILURE);
            }
            while(((returnVal=sem_post(emptySem))==-1) && errno==EINTR);
            if(returnVal==-1)
            {
                writeErrorMessage("Cannot wait on semaphore.\n", logFile);
                terminateZChildren();
                close(serverZPipe[0]);
                close(logFile);
                free(zWorkers);
                shm_unlink("/ResultMemory");
                munmap("/ResultMemory", sizeof(int)*2);
                shm_unlink("/Requests");
                munmap("/Requests", sizeof(struct requestQueue));
                exit(EXIT_FAILURE);
            }
        }

        close(serverZPipe[0]);
        terminateZChildren();
        close(logFile);
        free(zWorkers);
        shm_unlink("/ResultMemory");
        munmap("/ResultMemory", sizeof(int)*2);
        shm_unlink("/Requests");
        munmap("/Requests", sizeof(struct requestQueue));
        exit(EXIT_SUCCESS);
    }

    if(sigintFlag==1)
    {
        writeErrorMessage("SIGINT received, no requests were received yet. Terminating serverZ and exiting server Y.\n", logFile);
        close(serverZPipe[1]);
        terminateChildren();

        close(logFile);
        close(fifoFile);
        close(fifoFakeWriter);
        unlink(fifoFileName);
        shm_unlink("/ResultMemory");
        munmap("/ResultMemory", sizeof(int));
        return 0;
    }

    //Initiate workers
    childrenIDs=(struct childStatus*)malloc(sizeof(struct childStatus)*poolSize);
    if(childrenIDs==NULL)
    {
        writeErrorMessage("Cannot allocate memory for childrenIDs\n", logFile);
        close(serverZPipe[1]);
        terminateChildren();
        close(logFile);
        close(fifoFile);
        close(fifoFakeWriter);
        unlink(fifoFileName);
        shm_unlink("/ResultMemory");
        munmap("/ResultMemory", sizeof(int));
        exit(EXIT_FAILURE);
    }

    //SIGUSR1 handler
    struct sigaction sigusr1Act={0};
    sigemptyset(&sigusr1Act.sa_mask);
    sigaddset(&sigusr1Act.sa_mask, SIGINT);
    sigusr1Act.sa_sigaction=sigusr1Handler;
    sigusr1Act.sa_flags=SA_SIGINFO;
    while(((returnVal=sigaction(SIGRTMIN, &sigusr1Act, NULL))==-1) && errno==EINTR);
    if(returnVal==-1)
    {
        writeErrorMessage("Cannot assign signal handler.\n", logFile);
        close(serverZPipe[1]);
        terminateChildren();
        close(logFile);
        close(fifoFile);
        close(fifoFakeWriter);
        unlink(fifoFileName);
        free(childrenIDs);
        shm_unlink("/ResultMemory");
        munmap("/ResultMemory", sizeof(int));
        exit(EXIT_FAILURE);
    }

    //Creating workers
    for(int i=0; i<poolSize; ++i)
    {
        if(sigintFlag==1)
        {
            writeErrorMessage("SIGINT received, terminating Z and exiting server Y. No requests were received.\n", logFile);
            for(int i=0; i<workerCount; ++i)
                close(childrenIDs[i].pipoFileDes[1]);
            close(serverZPipe[1]);

            terminateChildren();

            close(logFile);
            close(fifoFile);
            close(fifoFakeWriter);
            unlink(fifoFileName);
            shm_unlink("/ResultMemory");
            munmap("/ResultMemory", sizeof(int));
            free(childrenIDs);

            return 0;
        }

        while(((returnVal=pipe(childrenIDs[i].pipoFileDes))==-1) && errno==EINTR);
        if(returnVal==-1)
        {
            writeErrorMessage("Pipe failed.\n", logFile);
            close(serverZPipe[1]);
            for(int i=0; i<workerCount; ++i)
                close(childrenIDs[i].pipoFileDes[1]);
            terminateChildren();
            close(logFile);
            close(fifoFile);
            close(fifoFakeWriter);
            unlink(fifoFileName);
            free(childrenIDs);
            shm_unlink("/ResultMemory");
            munmap("/ResultMemory", sizeof(int));
            exit(EXIT_FAILURE);
        }

        workerCount++;
        int childID=fork();
        if(childID==-1)
        {
            writeErrorMessage("Fork failed.\n", logFile);
            workerCount--;
            for(int i=0; i<workerCount; ++i)
                close(childrenIDs[i].pipoFileDes[1]);
            close(serverZPipe[1]);
            terminateChildren();
            close(logFile);
            close(fifoFile);
            close(fifoFakeWriter);
            unlink(fifoFileName);
            free(childrenIDs);
            shm_unlink("/ResultMemory");
            munmap("/ResultMemory", sizeof(int));
            exit(EXIT_FAILURE);
        }
        else if(childID>0)
        {
            childrenIDs[i].childID=childID;
            childrenIDs[i].free=true;
            if(close(childrenIDs[i].pipoFileDes[0])==-1)
            {
                writeErrorMessage("Close read ends of pipos failed.\n", logFile);
                for(int i=0; i<workerCount; ++i)
                    close(childrenIDs[i].pipoFileDes[1]);
                close(serverZPipe[1]);
                terminateChildren();
                close(logFile);
                close(fifoFile);
                close(fifoFakeWriter);
                unlink(fifoFileName);
                free(childrenIDs);
                shm_unlink("/ResultMemory");
                munmap("/ResultMemory", sizeof(int));
                exit(EXIT_FAILURE);
            }
        }
        else if(childID==0)
        {
            close(fifoFakeWriter);
            close(fifoFile);
            close(serverZPipe[1]);
            int index=i;
            if(close(childrenIDs[index].pipoFileDes[1])==-1)
            {
                writeErrorMessage("Close write end of pipo failed.\n", logFile);
                close(logFile);
                close(childrenIDs[index].pipoFileDes[0]);
                shm_unlink("/ResultMemory");
                munmap("/ResultMemory", sizeof(int));
                exit(EXIT_FAILURE);
            }

            if(sigintFlag==1)
            {
                writeErrorMessage("SIGINT received by server Y worker. Terminating the worker.\n", logFile);

                close(logFile);
                close(childrenIDs[index].pipoFileDes[0]);
                shm_unlink("/ResultMemory");
                munmap("/ResultMemory", sizeof(int));

                return 0;
            }

            //Worker processing here
            while(true)
            {
                //Read from pipe
                int clientPID=-1;
                int clientMatrixSize=-1;
                char* readNumber=NULL;
                int** matrix=NULL;
                int rowCount=0;
                int columnCount=0;
                int currentDigit=0;
                while(true)
                {
                    int bytesread;
                    char readChar[1];
                    while(((bytesread=read(childrenIDs[index].pipoFileDes[0], readChar, 1))==-1) && errno==EINTR);
                    if(bytesread<0)
                    {
                        writeErrorMessage("Read from worker pipo failed.\n", logFile);
                        freePointer(readNumber);
                        close(logFile);
                        close(childrenIDs[index].pipoFileDes[0]);
                        shm_unlink("/ResultMemory");
                        munmap("/ResultMemory", sizeof(int));
                        if(matrix!=NULL)
                        {
                            for(int i=0; i<clientMatrixSize; ++i)
                                free(matrix[i]);
                            free(matrix);
                        }
                        exit(EXIT_FAILURE);
                    }
                    else if(bytesread==0)
                        break;
                    else if(readChar[0]==' ' && clientPID==-1)
                    {
                        clientPID=atoi(readNumber);
                        free(readNumber);
                        readNumber=NULL;
                        currentDigit=0;
                    }
                    else if(readChar[0]==' ' && clientMatrixSize==-1)
                    {
                        clientMatrixSize=atoi(readNumber);
                        free(readNumber);
                        readNumber=NULL;

                        matrix=(int**)malloc(clientMatrixSize*sizeof(int*));
                        for(int i=0; i<clientMatrixSize; ++i)
                            matrix[i]=(int*)malloc(clientMatrixSize*sizeof(int));
                        currentDigit=0;
                    }
                    else if(readChar[0]=='\n' || readChar[0]==' ')
                    {
                        matrix[rowCount][columnCount]=atoi(readNumber);
                        free(readNumber);
                        readNumber=NULL;

                        columnCount++;
                        if(columnCount==clientMatrixSize)
                        {
                            columnCount=0;
                            rowCount++;
                        }
                        currentDigit=0;

                        if(readChar[0]=='\n')
                            break;
                    }
                    else if(readChar[0]!='\r')
                    {
                        readNumber=addDigit(*readChar, &readNumber, &currentDigit);
                        if(readNumber==NULL)
                        {
                            writeErrorMessage("Cannot allocate memory for readNumber in worker.\n", logFile);
                            freePointer(readNumber);
                            close(logFile);
                            close(childrenIDs[index].pipoFileDes[0]);
                            shm_unlink("/ResultMemory");
                            munmap("/ResultMemory", sizeof(int));
                            if(matrix!=NULL)
                            {
                                for(int i=0; i<clientMatrixSize; ++i)
                                    free(matrix[i]);
                                free(matrix);
                            }
                            exit(EXIT_FAILURE);
                        }
                    }
                }

                if(sigintFlag==1)
                {
                    writeErrorMessage("SIGINT received by server Y worker. Terminating the worker.\n", logFile);

                    close(logFile);
                    close(childrenIDs[index].pipoFileDes[0]);
                    shm_unlink("/ResultMemory");
                    munmap("/ResultMemory", sizeof(int));
                    if(matrix!=NULL)
                    {
                        for(int i=0; i<clientMatrixSize; ++i)
                            free(matrix[i]);
                        free(matrix);
                    }

                    return 0;
                }

                if(matrix!=NULL)
                {
                    bool result=isInvertible(matrix, clientMatrixSize);

                    if(result==true)
                    {
                        if(sem_wait(resultSharedMemSem)==-1)
                        {
                            writeErrorMessage("Cannot wait on resultSharedMem semaphore\n", logFile);
                            sem_post(resultSharedMemSem);
                            close(logFile);
                            close(childrenIDs[index].pipoFileDes[0]);
                            shm_unlink("/ResultMemory");
                            munmap("/ResultMemory", sizeof(int));
                            if(matrix!=NULL)
                            {
                                for(int i=0; i<clientMatrixSize; ++i)
                                    free(matrix[i]);
                                free(matrix);
                            }
                            exit(EXIT_FAILURE);
                        }

                        (*resultMemAdd)++;

                        if(sem_post(resultSharedMemSem)==-1)
                        {
                            writeErrorMessage("Cannot post on resultSharedMem semaphore\n", logFile);
                            sem_post(resultSharedMemSem);
                            close(logFile);
                            close(childrenIDs[index].pipoFileDes[0]);
                            shm_unlink("/ResultMemory");
                            munmap("/ResultMemory", sizeof(int));
                            if(matrix!=NULL)
                            {
                                for(int i=0; i<clientMatrixSize; ++i)
                                    free(matrix[i]);
                                free(matrix);
                            }
                            exit(EXIT_FAILURE);
                        }
                    }

                    //Locking the log file for writing
                    struct flock lock;
                    memset(&lock, 0, sizeof(lock));
                    lock.l_type=F_WRLCK;
                    while(((fcntl(logFile, F_SETLKW, &lock))==-1) && errno==EINTR);

                    //Writing second output
                    char workerOutput[300];
                    arrangeWorkerOutput2(workerOutput, clientPID, result, false);
                    while(((byteswritten=write(logFile, workerOutput, strlen(workerOutput)))==-1) && (errno==EINTR));
                    if(byteswritten==-1)
                    {
                        lock.l_type=F_UNLCK;
                        while(((fcntl(logFile, F_SETLKW, &lock))==-1) && errno==EINTR);
                        writeErrorMessage("Second worker output write failed.\n", logFile);
                        sem_post(resultSharedMemSem);
                        close(logFile);
                        close(childrenIDs[index].pipoFileDes[0]);
                        shm_unlink("/ResultMemory");
                        munmap("/ResultMemory", sizeof(int));
                        if(matrix!=NULL)
                        {
                            for(int i=0; i<clientMatrixSize; ++i)
                                free(matrix[i]);
                            free(matrix);
                        }
                        exit(EXIT_FAILURE);
                    }

                    lock.l_type=F_UNLCK;
                    while(((fcntl(logFile, F_SETLKW, &lock))==-1) && errno==EINTR);

                    //Sending the response
                    char clientFifoPath[100];
                    char buffer[100];

                    sprintf(buffer, "/tmp/%d", clientPID);
                    strcpy(clientFifoPath, buffer);
                    strcat(clientFifoPath, "FIFO.fifo");

                    int clientFifoFile=openFile(clientFifoPath, O_WRONLY, 0);
                    if(clientFifoFile==-1)
                    {
                        writeErrorMessage("Opening client fifo file failed.\n", logFile);
                        close(logFile);
                        close(childrenIDs[index].pipoFileDes[0]);
                        shm_unlink("/ResultMemory");
                        munmap("/ResultMemory", sizeof(int));
                        if(matrix!=NULL)
                        {
                            for(int i=0; i<clientMatrixSize; ++i)
                                free(matrix[i]);
                            free(matrix);
                        }
                        exit(EXIT_FAILURE);
                    }

                    char response[5];
                    if(result==true)
                        strcpy(response, "Yes");
                    else
                        strcpy(response, "No");

                    if(sigintFlag==1)
                    {
                        writeErrorMessage("SIGINT received by server Y worker. Terminating the worker.\n", logFile);

                        close(logFile);
                        close(childrenIDs[index].pipoFileDes[0]);
                        shm_unlink("/ResultMemory");
                        munmap("/ResultMemory", sizeof(int));
                        if(matrix!=NULL)
                        {
                            for(int i=0; i<clientMatrixSize; ++i)
                                free(matrix[i]);
                            free(matrix);
                        }
                        close(clientFifoFile);

                        return 0;
                    }

                    sleep(sleepTime);

                    while(((byteswritten=write(clientFifoFile, response, strlen(response)))==-1) && errno==EINTR);
                    if(byteswritten<0)
                    {
                        writeErrorMessage("Write response to client fifo file failed.\n", logFile);
                        close(logFile);
                        close(childrenIDs[index].pipoFileDes[0]);
                        shm_unlink("/ResultMemory");
                        munmap("/ResultMemory", sizeof(int));
                        if(matrix!=NULL)
                        {
                            for(int i=0; i<clientMatrixSize; ++i)
                                free(matrix[i]);
                            free(matrix);
                        }
                        close(clientFifoFile);
                        exit(EXIT_FAILURE);
                    }

                    close(clientFifoFile);
                    if(matrix!=NULL)
                    {
                        for(int i=0; i<clientMatrixSize; ++i)
                            free(matrix[i]);
                        free(matrix);
                    }

                    //Sending SIGUSR1
                    kill(getppid(), SIGRTMIN);
                }
            }

            close(logFile);
            close(childrenIDs[i].pipoFileDes[0]);
            shm_unlink("/ResultMemory");
            munmap("/ResultMemory", sizeof(int));
            exit(EXIT_SUCCESS);
        }
    }

    //Start the real processing
    int totalRequestsHandled=0;
    int forwardedRequestCount=0;
    while(true)
    {
        //Read from server fifo
        char* readRequest=NULL;
        char* readNumber=NULL;
        int clientPID=-1;
        int clientMatrixSize=-1;
        int size=0;
        int size2=0;
        while(true)
        {
            char readChar[1];
            int bytesread;
            while(((bytesread=read(fifoFile, readChar, 1))==-1) && errno==EINTR);
            if(bytesread<0)
            {
                writeErrorMessage("Read from server fifo failed.\n", logFile);
                for(int i=0; i<poolSize; ++i)
                    close(childrenIDs[i].pipoFileDes[1]);
                close(serverZPipe[1]);
                terminateChildren();

                free(childrenIDs);
                close(logFile);
                close(fifoFile);
                close(fifoFakeWriter);
                unlink(fifoFileName);
                freePointer(readRequest);
                freePointer(readNumber);
                shm_unlink("/ResultMemory");
                munmap("/ResultMemory", sizeof(int));
                exit(EXIT_FAILURE);
            }
            else if(bytesread==0)
                break;
            else
            {
                if((clientPID==-1 || clientMatrixSize==-1) && readChar[0]!=' ' && readChar[0]!='\r')
                    readNumber=addDigit(readChar[0], &readNumber, &size2);
                readRequest=addDigit(readChar[0], &readRequest, &size);
                if(readRequest==NULL || ((readNumber==NULL) && ((clientPID==-1) || (clientMatrixSize==-1)) ) )
                {
                    writeErrorMessage("Cannot allocate memory for request in serverY.\n", logFile);
                    for(int i=0; i<poolSize; ++i)
                        close(childrenIDs[i].pipoFileDes[1]);
                    close(serverZPipe[1]);
                    terminateChildren();

                    free(childrenIDs);
                    close(logFile);
                    close(fifoFile);
                    close(fifoFakeWriter);
                    unlink(fifoFileName);
                    freePointer(readNumber);
                    freePointer(readRequest);
                    shm_unlink("/ResultMemory");
                    munmap("/ResultMemory", sizeof(int));
                    exit(EXIT_FAILURE);
                }
                if(readChar[0]=='\n')
                    break;
                else if(readChar[0]==' ' && clientPID==-1)
                {
                    clientPID=atoi(readNumber);
                    free(readNumber);
                    readNumber=NULL;
                    size2=0;
                }
                else if(readChar[0]==' ' && clientMatrixSize==-1)
                {
                    clientMatrixSize=atoi(readNumber);
                    free(readNumber);
                    readNumber=NULL;
                    size2=0;
                }
            }
        }
        free(readNumber);

        if(sigintFlag==1)
        {
            char sigintOutput[300];
            arrangeSigintOutput(sigintOutput, totalRequestsHandled+forwardedRequestCount, forwardedRequestCount, (*resultMemAdd), false);
            writeErrorMessage(sigintOutput, logFile);

            for(int i=0; i<poolSize; ++i)
                close(childrenIDs[i].pipoFileDes[1]);
            close(serverZPipe[1]);
            terminateChildren();

            free(childrenIDs);
            close(logFile);
            close(fifoFile);
            close(fifoFakeWriter);
            unlink(fifoFileName);
            shm_unlink("/ResultMemory");
            munmap("/ResultMemory", sizeof(int));
            freePointer(readRequest);

            return 0;
        }

        //Finding a free child an passing the request to the child
        if(poolBusy!=poolSize && readRequest!=NULL)
        {
            for(int i=0; i<poolSize; ++i)
                if(childrenIDs[i].free==true)
                {
                    poolBusy++;
                    childrenIDs[i].free=false;
                    //Locking the log file for writing
                    struct flock lock;
                    memset(&lock, 0, sizeof(lock));
                    lock.l_type=F_WRLCK;
                    while(((fcntl(logFile, F_SETLKW, &lock))==-1) && errno==EINTR);

                    //Writing first output
                    char workerOutput[300];
                    arrangeWorkerOutput1(workerOutput, childrenIDs[i].childID, clientPID, clientMatrixSize, false, poolBusy, poolSize);
                    while(((byteswritten=write(logFile, workerOutput, strlen(workerOutput)))==-1) && (errno==EINTR));
                    if(byteswritten==-1)
                    {
                        lock.l_type=F_UNLCK;
                        while(((fcntl(logFile, F_SETLKW, &lock))==-1) && errno==EINTR);
                        writeErrorMessage("First Worker Write failed.\n", logFile);
                        for(int i=0; i<poolSize; ++i)
                            close(childrenIDs[i].pipoFileDes[1]);
                        close(serverZPipe[1]);
                        terminateChildren();

                        free(childrenIDs);
                        close(logFile);
                        close(fifoFile);
                        close(fifoFakeWriter);
                        unlink(fifoFileName);
                        freePointer(readRequest);
                        shm_unlink("/ResultMemory");
                        munmap("/ResultMemory", sizeof(int));
                        exit(EXIT_FAILURE);
                    }

                    //Unlocking the log file
                    lock.l_type=F_UNLCK;
                    while(((fcntl(logFile, F_SETLKW, &lock))==-1) && errno==EINTR);

                    if(sigintFlag==1)
                    {
                        char sigintOutput[300];
                        arrangeSigintOutput(sigintOutput, totalRequestsHandled, forwardedRequestCount, (*resultMemAdd), false);
                        writeErrorMessage(sigintOutput, logFile);
                        for(int i=0; i<poolSize; ++i)
                            close(childrenIDs[i].pipoFileDes[1]);
                        close(serverZPipe[1]);
                        terminateChildren();

                        free(childrenIDs);
                        close(logFile);
                        close(fifoFile);
                        close(fifoFakeWriter);
                        unlink(fifoFileName);
                        shm_unlink("/ResultMemory");
                        munmap("/ResultMemory", sizeof(int));
                        freePointer(readRequest);

                        return 0;
                    }

                    while((byteswritten=(write(childrenIDs[i].pipoFileDes[1], readRequest, strlen(readRequest)))==-1) && errno==EINTR);
                    if(byteswritten<0)
                    {
                        writeErrorMessage("Cannot write to worker pipo\n", logFile);
                        for(int i=0; i<poolSize; ++i)
                            close(childrenIDs[i].pipoFileDes[1]);
                        close(serverZPipe[1]);
                        terminateChildren();

                        free(childrenIDs);
                        close(logFile);
                        close(fifoFile);
                        close(fifoFakeWriter);
                        unlink(fifoFileName);
                        freePointer(readRequest);
                        shm_unlink("/ResultMemory");
                        munmap("/ResultMemory", sizeof(int));
                        exit(EXIT_FAILURE);
                    }
                    totalRequestsHandled++;

                    break;
                }
        }
        else if(readRequest!=NULL)  //Send to server Z
        {
            forwardedRequestCount++;
            //Server Z forwarding output
            //Locking the log file for writing
            struct flock lock;
            memset(&lock, 0, sizeof(lock));
            lock.l_type=F_WRLCK;
            while(((fcntl(logFile, F_SETLKW, &lock))==-1) && errno==EINTR);

            char forwardOutput[300];
            arrangeForwardOutput(forwardOutput, clientPID, clientMatrixSize);
            while(((byteswritten=write(logFile, forwardOutput, strlen(forwardOutput)))==-1) && (errno==EINTR));
            if(byteswritten==-1)
            {
                lock.l_type=F_UNLCK;
                while(((fcntl(logFile, F_SETLKW, &lock))==-1) && errno==EINTR);
                writeErrorMessage("Forwards write failed.\n", logFile);
                for(int i=0; i<poolSize; ++i)
                    close(childrenIDs[i].pipoFileDes[1]);
                close(serverZPipe[1]);
                terminateChildren();
                free(childrenIDs);
                close(logFile);
                close(fifoFile);
                close(fifoFakeWriter);
                unlink(fifoFileName);
                freePointer(readRequest);
                shm_unlink("/ResultMemory");
                munmap("/ResultMemory", sizeof(int));
                exit(EXIT_FAILURE);
            }

            if(sigintFlag==1)
            {
                char sigintOutput[300];
                arrangeSigintOutput(sigintOutput, totalRequestsHandled, forwardedRequestCount, (*resultMemAdd), false);
                writeErrorMessage(sigintOutput, logFile);
                for(int i=0; i<poolSize; ++i)
                    close(childrenIDs[i].pipoFileDes[1]);
                close(serverZPipe[1]);
                terminateChildren();
                free(childrenIDs);
                close(logFile);
                close(fifoFile);
                close(fifoFakeWriter);
                unlink(fifoFileName);
                freePointer(readRequest);
                shm_unlink("/ResultMemory");
                munmap("/ResultMemory", sizeof(int));

                return 0;
            }

            //Unlocking the log file
            lock.l_type=F_UNLCK;
            while(((fcntl(logFile, F_SETLKW, &lock))==-1) && errno==EINTR);

            while((byteswritten=(write(serverZPipe[1], readRequest, strlen(readRequest)))==-1) && errno==EINTR);
            if(byteswritten<0)
            {
                writeErrorMessage("Cannot write to server z pipo\n", logFile);

                for(int i=0; i<poolSize; ++i)
                    close(childrenIDs[i].pipoFileDes[1]);
                close(serverZPipe[1]);
                terminateChildren();
                free(childrenIDs);
                close(logFile);
                close(fifoFile);
                close(fifoFakeWriter);
                unlink(fifoFileName);
                freePointer(readRequest);
                shm_unlink("/ResultMemory");
                munmap("/ResultMemory", sizeof(int));
                exit(EXIT_FAILURE);
            }
        }

        free(readRequest);
    }

    for(int i=0; i<poolSize; ++i)
        close(childrenIDs[i].pipoFileDes[1]);
    close(serverZPipe[1]);
    terminateChildren();
    free(childrenIDs);
    close(logFile);
    close(fifoFile);
    close(fifoFakeWriter);
    unlink(fifoFileName);
    shm_unlink("/ResultMemory");
    munmap("/ResultMemory", sizeof(int));

    return 0;
}

void checkArgc(int count)
{
    if(count!=11)
    {
        char errorMessage[]="You need exactly 11 command line arguments (first being auto inputed) to run client. Please try again.\n";
        while((write(STDERR_FILENO, errorMessage, sizeof(errorMessage)-1)==-1) && (errno==EINTR));
        exit(EXIT_FAILURE);
    }
}

int retrieveArguments(int count, char* arguements[], char** fifoPath, char** logPath, int* pool1, int* pool2, int* sleep)
{
    int option;
    while((option=getopt(count, arguements, "s:o:p:r:t:"))!=-1)
        switch(option)
        {
            case 's':
                *fifoPath=optarg;
                break;
            case 'o':
                *logPath=optarg;
                break;
            case 'p':
                *pool1=atoi(optarg);
                break;
            case 'r':
                *pool2=atoi(optarg);
                break;
            case 't':
                *sleep=atoi(optarg);
                break;
            default:
                return -1;
        }

    return 0;
}

int becomeDaemon(int flags)
{
    //Go to background
    int id=fork();
    if(id==-1)
        return -1;
    else if(id!=0)
        exit(EXIT_SUCCESS);

    //Become the leader of the new session
    if (setsid() == -1)
        return -1;

    //No more session leader
    id=fork();
    if(id==-1)
        return -1;
    else if(id!=0)
        exit(EXIT_SUCCESS);

    int maxFD, FD;
    if (!(flags & BD_UMASK))    //Clear file modes
        umask(0);
    if (BD_CHDIR) //Change root directory
        chdir("/");
    if (!(flags & BD_CLOSE_FILES))   //Close all opened files
    {
        maxFD = sysconf(_SC_OPEN_MAX);

        if(maxFD==-1)
            maxFD=BD_MAX_CLOSE;

        for(FD=0; FD<maxFD; FD++)
            close(FD);
    }
    if (!(flags & BD_REOPEN_STD_FDS))    //Make sure no controlling terminal can be opened again
    {
        close(STDIN_FILENO);
        FD=open("/dev/null", O_RDWR);

        if (FD!=STDIN_FILENO)
            return -1;
        if (dup2(STDIN_FILENO, STDOUT_FILENO)!=STDOUT_FILENO)
            return -1;
        if (dup2(STDIN_FILENO, STDERR_FILENO)!=STDERR_FILENO)
            return -1;
    }

    return 0;
}

void upSem()
{
    if(serverYSem!=NULL)
    {
        sem_post(serverYSem);
        sem_close(serverYSem);
        sem_unlink("/SeverYInit");
    }
    if(resultSharedMemSem!=NULL)
    {
        sem_close(resultSharedMemSem);
        sem_unlink("/resultSharedMemory");
    }
    if(requestsSem!=NULL)
    {
        sem_close(requestsSem);
        sem_unlink("/requestsSem");
    }
    if(emptySem!=NULL)
    {
        sem_close(emptySem);
        sem_unlink("/emptySem");
    }
    if(fullSem!=NULL)
    {
        sem_close(fullSem);
        sem_unlink("/fullSem");
    }
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

int createFifo(char* fifoPath)
{
    int returnVal;
    while((returnVal=mkfifo(fifoPath, S_IWUSR | S_IRUSR))==-1 && errno==EINTR);
    if(returnVal==-1 && errno!=EEXIST)
        return -1;
    else
        return 0;
}

void createMessage(char* startMessage, char* message)
{
    time_t currentTime;
    char res[32];
    struct tm *lt;

    time(&currentTime);
    lt=localtime(&currentTime);
    strftime(res, 32, default_format, lt);

    strcpy(message, res);
    strcat(message, ": ");
    strcat(message, startMessage);

}

void writeErrorMessage(char* startMessage, int logFd)
{
    char errorMessage[400];
    createMessage(startMessage, errorMessage);

    //Locking the log file for writing
    struct flock lock;
    memset(&lock, 0, sizeof(lock));
    lock.l_type=F_WRLCK;

    while(((fcntl(logFd, F_SETLKW, &lock))==-1) && errno==EINTR);
    while((write(logFd, errorMessage, strlen(errorMessage))==-1) && (errno==EINTR));

    //Unlocking the log file
    lock.l_type=F_UNLCK;
    while(((fcntl(logFd, F_SETLKW, &lock))==-1) && errno==EINTR);
}

void arrangeOutput1(char* output, char* fileName, int poolSize, int sleepTime)
{
    char buffer[100];
    char res[32];
    struct tm *lt;
    time_t currentTime;
    int startTime=(int)time(&currentTime);
    lt=localtime(&currentTime);

    strftime(res, 32, default_format, lt);
    strcpy(output, res);
    strcat(output, ": Server Y (");
    strcat(output, fileName);
    snprintf(buffer, 100, ", p=%d t=%d) started\n", poolSize, sleepTime);
    strcat(output, buffer);
}

void freePointer(void* pointer)
{
    if(pointer!=NULL)
        free(pointer);
}

char* addDigit(char newDigit, char** oldNumber, int* oldNumberSize)
{
    if(*oldNumberSize==0)
    {
        *oldNumber=(char*)malloc(2);
        if(oldNumber==NULL)
            return NULL;
        (*oldNumber)[0]=newDigit;
        (*oldNumber)[1]='\0';
        (*oldNumberSize)++;
        return *oldNumber;
    }

    char* backupNumber=(char*)malloc(*oldNumberSize);
    if(backupNumber==NULL)
        return NULL;

    for(int i=0; i<*oldNumberSize; ++i)
        backupNumber[i]=(*oldNumber)[i];

    *oldNumber=(char*)realloc(*oldNumber, (*oldNumberSize)+2);
    if(*oldNumber==NULL)
    {
        free(backupNumber);
        return(NULL);
    }
    for(int i=0; i<*oldNumberSize; ++i)
        (*oldNumber)[i]=backupNumber[i];
    (*oldNumber)[*oldNumberSize]=newDigit;
    (*oldNumber)[*oldNumberSize+1]='\0';

    (*oldNumberSize)++;

    free(backupNumber);

    return *oldNumber;
}

void sigusr1Handler(int signo, siginfo_t *si, void *data)
{
    for(int i=0; i<workerCount; ++i)
        if(childrenIDs[i].childID==si->si_pid && childrenIDs[i].free==false)
        {
            childrenIDs[i].free=true;
            poolBusy--;
        }
}

bool isInvertible(int** matrix, int size)
{
    if(determinant(matrix, size)==0)
        return false;
    else
        return true;
}

long long int determinant(int** matrix, int size)
{
    if(size==1)
        return matrix[0][0];
    else if(size==2)
        return (((long long int)matrix[0][0])*((long long int)matrix[1][1])-((long long int)matrix[0][1])*((long long int)matrix[1][0]));
    else
    {
        int** minorMatrix=(int**)malloc((size-1)*sizeof(int*));
        for(int i=0; i<size-1; ++i)
            minorMatrix[i]=(int*)malloc((size-1)*sizeof(int));
        long long int detSum=0;
        int minorRow=0;
        int minorCol=0;

        for(int currentRowIndex=0; currentRowIndex<size; ++currentRowIndex)
        {
            long long int member=(long long int)matrix[currentRowIndex][size-1];

            for(int i=0; i<size; ++i)
                for(int j=0; j<size; ++j)
                {
                    if(i==currentRowIndex)
                        break;
                    else if(j==size-1)
                        break;
                    else
                    {
                        minorMatrix[minorRow][minorCol]=(long long int)matrix[i][j];
                        minorCol++;
                        if(minorCol==size-1)
                        {
                            minorRow++;
                            minorCol=0;
                        }
                    }
                }

            long long int res=determinant(minorMatrix, size-1)*member;
            if((currentRowIndex+(size-1))%2!=0)
                res*=-1;

            detSum+=res;

            minorCol=0;
            minorRow=0;
        }

        for(int i=0; i<size-1; ++i)
            free(minorMatrix[i]);
        free(minorMatrix);

        return detSum;
    }
}

void arrangeWorkerOutput1(char* output, int worderPid, int clientPid, int matrixSize, bool isZ, int busy, int count)
{
    char buffer[300];
    char res[32];
    struct tm *lt;
    time_t currentTime;
    (int)time(&currentTime);
    lt=localtime(&currentTime);

    strftime(res, 32, default_format, lt);
    strcpy(output, res);

    if(isZ)
        strcat(output, ": Z");

    snprintf(buffer, 300, ": Worker PID#%d is handling client PID#%d, matrix size %dx%d, pool busy %d/%d\n", worderPid, clientPid, matrixSize, matrixSize, busy, count);
    strcat(output, buffer);
}

void arrangeWorkerOutput2(char* output, int clientPid, bool result, bool isZ)
{
    time_t currentTime;
    (int)time(&currentTime);
    char buffer[300];

    char res[32];
    struct tm *lt;

    lt=localtime(&currentTime);
    strftime(res, 32, default_format, lt);

    output[0]='\0';
    strcpy(output, res);

    if(isZ)
        strcat(output, ": Z");

    snprintf(buffer, 300, ": Worker PID#%d responding to client PID#%d: the matrix IS ", getpid(), clientPid);
    strcat(output, buffer);

    if(result==false)
        strcat(output, "NOT ");
    strcat(output, "invertible.\n");
}

void sigintHandler(int signum)
{
    if(opened==0)
    {
        unlink(fifoFileName);
        upSem();
        _exit(1);
    }
    else
        close(fifoFakeWriter);

    if(fullSem!=NULL)
        sem_post(fullSem);
    if(requestsSem!=NULL)
        sem_post(requestsSem);

    sigintFlag=1;
}

void terminateChildren()
{
    for(int i=0; i<workerCount; ++i)
        kill(childrenIDs[i].childID, SIGINT);

    if(serverZID!=-1)
    {
        workerCount++;
        kill(serverZID, SIGINT);
    }

    for(int i=0; i<workerCount; ++i)
        waitpid(-1, NULL, 0);

}

void terminateZChildren()
{
    for(int i=0; i<workerZCount; ++i)
        kill(zWorkers[i].childID, SIGINT);

    if(emptySem!=NULL)
        sem_post(emptySem);
    if(requestsSem!=NULL)
        sem_post(requestsSem);

    for(int i=0; i<workerZCount; ++i)
        waitpid(-1, NULL, 0);
}

void arrangeForwardOutput(char* output, int clientPID, int clientMatrixSize)
{
    time_t currentTime;
    (int)time(&currentTime);
    char buffer[300];

    char res[32];
    struct tm *lt;

    lt=localtime(&currentTime);
    strftime(res, 32, default_format, lt);
    strcpy(output, res);

    snprintf(buffer, 300, ": Forwarding request of client PID#%d to serverZ", clientPID);
    strcat(output, buffer);
    snprintf(buffer, 300, ", matrix size %dx%d, pool busy %d/%d\n", clientMatrixSize, clientMatrixSize, poolBusy, poolBusy);
    strcat(output, buffer);
}

void arrangeServerZOutput1(char* output, char* logFile, int sleepTime, int workerCount)
{
    time_t currentTime;
    (int)time(&currentTime);
    char buffer[300];

    char res[32];
    struct tm *lt;

    lt=localtime(&currentTime);
    strftime(res, 32, default_format, lt);
    strcpy(output, res);

    strcat(output, ": Z:Server Z (");
    strcat(output, logFile);

    snprintf(buffer, 150, ", t=%d, r=%d) started\n", sleepTime, workerCount);
    strcat(output, buffer);
}

void arrangeSigintOutput(char* output, int total, int forwarded, int invertibleCount, bool isZ)
{
    char buffer[300];

    strcpy(output, "");

    if(isZ==true)
        strcat(output, "Z: ");
    strcat(output, "SIGINT received, terminating Z and exiting server Y. Total requests handled: ");
    snprintf(buffer, 300, "%d, %d invertible %d not.", total, invertibleCount, total-invertibleCount);
    strcat(output, buffer);

    if(!isZ)
    {
        snprintf(buffer, 300, " %d requests were forwarded.\n", forwarded);
        strcat(output, buffer);
    }
    else
       strcat(output, "\n");
}

