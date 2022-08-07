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
#include <math.h>
#include <time.h>
#define PI 3.14159265359

struct requiredData{
    int startColumn;
    int endColumn;
    int threadNo;
};

struct transformResult{
    double** reelMatrix;
    double** imMatrix;
};

int sigintFlag=0;

//Result globals. These will be changed by threads
double** matrixC;
struct transformResult res;

//These wont change after first set
static const char default_format[] = "%b %d %Z %Y %H %M %S";
double** matrixA;
double** matrixB;
int size;

//Mutex related globals
pthread_mutex_t critMutex=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t arrivedCond=PTHREAD_COND_INITIALIZER;
int arrived=0;
int mCount=-1;

void checkArgc(int count);
int retrieveArguments(int count, char* arguements[], int* n, int* m, char** dataPath1, char** dataPath2, char** outputPath);
int openFile(char* filePath, int flags, mode_t mode);
int writeOutput(char* output, int fileFd);
void free2DMatrix(double** matrix, int size);
void* threadFunction(void* args);
void calculateCColumns(int startIndex, int endIndex);
void arrangeStartOutput(char* output);
void randezvousOutput(char* output, int no, struct timespec start);
void advancingOutput(char* output, int no);
void threadFinishOutput(char* output, int no, struct timespec start);
void processFinishOutput(char* output, struct timespec start);
void calculateTransform(int start, int end);
char* arrangeOutput(char** output);
void sigintHandler(int signum);

int main(int argc, char* argv[])
{
    setbuf(stdout, NULL);
    setenv("TZ", "Europe/Istanbul", 1);
    tzset();

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

    //Checking the command line arguments
    checkArgc(argc);

    //Retrieving the command line inputs
    int nCount=-1;
    char* dataFilePath1=NULL;
    char* dataFilePath2=NULL;
    char* outputFilePath=NULL;
    retrieveArguments(argc, argv, &nCount, &mCount, &dataFilePath1, &dataFilePath2, &outputFilePath);
    if(dataFilePath1==NULL || dataFilePath2==NULL || outputFilePath==NULL || nCount<=2 || mCount<2)
    {
        char errorMessage[]="Arguments are not provided properly. Please try again.\n";
        while((write(STDERR_FILENO, errorMessage, sizeof(errorMessage)-1)==-1) && (errno==EINTR));
        exit(EXIT_FAILURE);
    }

    size=pow(2, nCount);

    //Initializing the matrixes
    matrixA=(double**)malloc(size*sizeof(double*));
    if(matrixA==NULL)
    {
        perror("Cannot allocate memory for matrixA");
        exit(EXIT_FAILURE);
    }
    for(int i=0; i<size; i++)
    {
        matrixA[i]=(double*)malloc(size*sizeof(double));
        if(matrixA[i]==NULL)
        {
            perror("Cannot allocate memory for matrixA");
            free2DMatrix(matrixA, i);
            exit(EXIT_FAILURE);
        }
    }

    //Opening the first file
    int dataFile1=openFile(dataFilePath1, O_RDONLY, 0555);
    if(dataFile1<0)
    {
        perror("The first data file cannot be opened");
        free2DMatrix(matrixA, size);
        exit(EXIT_FAILURE);
    }

    //Reading from the first file and setting matrixA
    unsigned char readChar[1];
    int bytesread;
    int returnVal=0;
    int currentRow=0;
    int currentColumn=0;
    for(int i=0; i<size*size; ++i)
    {
        while(((bytesread=read(dataFile1, readChar, 1))==-1) && (errno==EINTR));
        if(sigintFlag==1)
        {
            writeOutput("SIGINT caught while reading first file\n", STDOUT_FILENO);
            close(dataFile1);
            free2DMatrix(matrixA, size);
            return 0;
        }
        if(bytesread<0)
        {
            perror("Cannot read from the first file");
            close(dataFile1);
            free2DMatrix(matrixA, size);
            exit(EXIT_FAILURE);
        }
        else if(bytesread==0)
        {
            writeOutput("There are not enough characters to read in the first data file", STDERR_FILENO);
            close(dataFile1);
            free2DMatrix(matrixA, size);
            exit(EXIT_FAILURE);
        }
        else
        {
            matrixA[currentRow][currentColumn]=(double)readChar[0];
            currentColumn++;
            if(currentColumn==size)
            {
                currentColumn=0;
                currentRow++;
            }
        }
    }

    close(dataFile1);
    currentRow=0;
    currentColumn=0;

    matrixB=(double**)malloc(size*sizeof(double*));
    if(matrixB==NULL)
    {
        perror("Cannot allocate memory for matrixB");
        free2DMatrix(matrixA, size);
        exit(EXIT_FAILURE);
    }
    for(int i=0; i<size; i++)
    {
        matrixB[i]=(double*)malloc(size*sizeof(double));
        if(matrixB[i]==NULL)
        {
            perror("Cannot allocate memory for matrixA");
            free2DMatrix(matrixA, size);
            free2DMatrix(matrixB, i);
            exit(EXIT_FAILURE);
        }
    }

    //Opening the second file
    int dataFile2=openFile(dataFilePath2, O_RDONLY, 0555);
    if(dataFile2<0)
    {
        perror("The second data file cannot be opened");
        free2DMatrix(matrixA, size);
        free2DMatrix(matrixB, size);
        exit(EXIT_FAILURE);
    }

    //Reading from the second file and setting matrixB
    for(int i=0; i<size*size; ++i)
    {
        while(((bytesread=read(dataFile2, readChar, 1))==-1) && (errno==EINTR));
        if(sigintFlag==1)
        {
            writeOutput("SIGINT caught while reading second file\n", STDOUT_FILENO);
            close(dataFile2);
            free2DMatrix(matrixA, size);
            free2DMatrix(matrixB, size);
            return 0;
        }
        if(bytesread<0)
        {
            perror("Cannot read from the second file");
            close(dataFile2);
            free2DMatrix(matrixA, size);
            free2DMatrix(matrixB, size);
            exit(EXIT_FAILURE);
        }
        else if(bytesread==0)
        {
            writeOutput("There are not enough characters to read in the second data file", STDERR_FILENO);
            close(dataFile2);
            free2DMatrix(matrixA, size);
            free2DMatrix(matrixB, size);
            exit(EXIT_FAILURE);
        }
        else
        {
            matrixB[currentRow][currentColumn]=(double)readChar[0];
            currentColumn++;
            if(currentColumn==size)
            {
                currentColumn=0;
                currentRow++;
            }
        }
    }

    close(dataFile2);

    struct timespec processStart;
    clock_gettime(CLOCK_MONOTONIC_RAW, &processStart);

    //Allocate memory for matrixC
    matrixC=(double**)malloc(size*sizeof(double*));
    if(matrixC==NULL)
    {
        perror("Cannot allocate memory for matrixC");
        free2DMatrix(matrixA, size);
        free2DMatrix(matrixB, size);
        exit(EXIT_FAILURE);
    }
    for(int i=0; i<size; i++)
    {
        matrixC[i]=(double*)malloc(size*sizeof(double));
        if(matrixC[i]==NULL)
        {
            perror("Cannot allocate memory for matrixA");
            free2DMatrix(matrixA, size);
            free2DMatrix(matrixB, size);
            free2DMatrix(matrixC, i);
            exit(EXIT_FAILURE);
        }
    }

    //Allocate memory for result struct
    res.reelMatrix=(double**)malloc(size*sizeof(double*));
    if(res.reelMatrix==NULL)
    {
        perror("Cannot allocate memory for result matrix");
        free2DMatrix(matrixA, size);
        free2DMatrix(matrixB, size);
        free2DMatrix(matrixC, size);
        exit(EXIT_FAILURE);
    }
    for(int i=0; i<size; i++)
    {
        res.reelMatrix[i]=(double*)malloc(size*sizeof(double));
        if(res.reelMatrix[i]==NULL)
        {
            perror("Cannot allocate memory for result matrix");
            free2DMatrix(matrixA, size);
            free2DMatrix(matrixB, size);
            free2DMatrix(matrixC, size);
            free2DMatrix(res.reelMatrix, i);
            exit(EXIT_FAILURE);
        }
    }

    res.imMatrix=(double**)malloc(size*sizeof(double*));
    if(res.imMatrix==NULL)
    {
        perror("Cannot allocate memory for result matrix");
        free2DMatrix(matrixA, size);
        free2DMatrix(matrixB, size);
        free2DMatrix(matrixC, size);
        free2DMatrix(res.reelMatrix, size);
        exit(EXIT_FAILURE);
    }
    for(int i=0; i<size; i++)
    {
        res.imMatrix[i]=(double*)malloc(size*sizeof(double));
        if(res.imMatrix[i]==NULL)
        {
            perror("Cannot allocate memory for result matrix");
            free2DMatrix(matrixA, size);
            free2DMatrix(matrixB, size);
            free2DMatrix(matrixC, size);
            free2DMatrix(res.reelMatrix, size);
            free2DMatrix(res.imMatrix, i);
            exit(EXIT_FAILURE);
        }
    }

    char output[1000];
    arrangeStartOutput(output);
    if(writeOutput(output, STDOUT_FILENO)==-1)
        perror("Cannot write the start output of the process");

    if(sigintFlag==1)
    {
        writeOutput("SIGINT caught before creating threads\n", STDOUT_FILENO);

        free2DMatrix(matrixA, size);
        free2DMatrix(matrixB, size);
        free2DMatrix(matrixC, size);
        free2DMatrix(res.reelMatrix, size);
        free2DMatrix(res.imMatrix, size);
        return 0;
    }

    //Creating the threads
    pthread_t threads[mCount];
    struct requiredData threadData[mCount];
    for(int i=0; i<mCount; ++i)
    {
        threadData[i].startColumn=((float)size/(float)mCount)*i;
        threadData[i].endColumn=((float)size/(float)mCount)*(i+1);
        threadData[i].threadNo=i+1;
        returnVal=pthread_create(&threads[i] , NULL, threadFunction, &(threadData[i]));
        if(returnVal!=0)
        {
            writeOutput("Failed to create consumer thread\n", STDERR_FILENO);
            sigintFlag=1;
            free2DMatrix(matrixA, size);
            free2DMatrix(matrixB, size);
            free2DMatrix(matrixC, size);
            free2DMatrix(res.reelMatrix, size);
            free2DMatrix(res.imMatrix, size);

            exit(EXIT_FAILURE);
        }
    }

    //Waiting for the threads
    for(int i=0; i<mCount; ++i)
        pthread_join(threads[i], NULL);

    //Freeing unnecessary matrixes
    free2DMatrix(matrixA, size);
    free2DMatrix(matrixB, size);
    free2DMatrix(matrixC, size);

    //Opening the output file
    int outputFile=openFile(outputFilePath, O_RDWR | O_CREAT | O_TRUNC, 0777);
    if(outputFile<0)
    {
        perror("The output file cannot be opened");
        free2DMatrix(res.reelMatrix, size);
        free2DMatrix(res.imMatrix, size);
        exit(EXIT_FAILURE);
    }

    //Writing to the output file
    char* finishOutput=NULL;
    finishOutput=arrangeOutput(&finishOutput);
    if(finishOutput==NULL && sigintFlag!=1)
    {
        perror("Cannot allocate memory");
        close(outputFile);
        free2DMatrix(res.reelMatrix, size);
        free2DMatrix(res.imMatrix, size);
        exit(EXIT_FAILURE);
    }

    if(sigintFlag==1)
    {
        writeOutput("SIGINT caught while preparing the output\n", STDOUT_FILENO);
        close(outputFile);
        free(finishOutput);
        free2DMatrix(res.reelMatrix, size);
        free2DMatrix(res.imMatrix, size);
        return 0;
    }

    if(writeOutput(finishOutput, outputFile)==-1)
        perror("Cannot write the output to the output file");

    processFinishOutput(output, processStart);
    if(writeOutput(output, STDOUT_FILENO)==-1)
        perror("Cannot write the finish output of process");

    close(outputFile);
    free(finishOutput);
    free2DMatrix(res.reelMatrix, size);
    free2DMatrix(res.imMatrix, size);

    return 0;
}

void checkArgc(int count)
{
    if(count!=11)
    {
        char errorMessage[]="You need exactly 11 command line arguments (first being auto inputed) to run the program. Please try again.\n";
        while((write(STDERR_FILENO, errorMessage, sizeof(errorMessage)-1)==-1) && (errno==EINTR));
        exit(EXIT_FAILURE);
    }
}

int retrieveArguments(int count, char* arguements[], int* n, int* m, char** dataPath1, char** dataPath2, char** outputPath)
{
    int option;
    while((option=getopt(count, arguements, "i:j:o:n:m:"))!=-1)
        switch(option)
        {
            case 'i':
                *dataPath1=optarg;
                break;
            case 'j':
                *dataPath2=optarg;
                break;
            case 'o':
                *outputPath=optarg;
                break;
            case 'n':
                *n=atoi(optarg);
                break;
            case 'm':
                *m=atoi(optarg);
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

void free2DMatrix(double** matrix, int size)
{
    for(int i=0; i<size; ++i)
        free(matrix[i]);
    free(matrix);
}

void* threadFunction(void* args)
{
    struct requiredData* dataSet=args;

    struct timespec threadStart;
    clock_gettime(CLOCK_MONOTONIC_RAW, &threadStart);

    char output[1000];

    //Calculating matrixC
    calculateCColumns(dataSet->startColumn, dataSet->endColumn);

    if(sigintFlag==1)
    {
        writeOutput("SIGINT caught by thread\n", STDOUT_FILENO);
        pthread_cond_broadcast(&arrivedCond);
        return NULL;
    }

    //Wanting to advance
    int returnVal=0;
    returnVal=pthread_mutex_lock(&critMutex);
    if(returnVal!=0 && sigintFlag!=1)
    {
        perror("Cannot lock mutex");
        sigintFlag=1;
        pthread_cond_broadcast(&arrivedCond);
        return NULL;
    }

    if(sigintFlag==1)
    {
        writeOutput("SIGINT caught by thread\n", STDOUT_FILENO);
        pthread_cond_broadcast(&arrivedCond);
        pthread_mutex_unlock(&critMutex);
        return NULL;
    }

    randezvousOutput(output, dataSet->threadNo, threadStart);
    if(writeOutput(output, STDOUT_FILENO)==-1)
        perror("Cannot write the randezvous reach output of the thread");

    arrived++;

    while(arrived<mCount)
    {
        pthread_cond_wait(&arrivedCond, &critMutex);
        if(returnVal!=0 && sigintFlag!=1)
        {
            perror("Cannot wait on condition");
            sigintFlag=1;
            pthread_cond_broadcast(&arrivedCond);
            pthread_mutex_unlock(&critMutex);

            return NULL;
        }

        if(sigintFlag==1)
        {
            writeOutput("SIGINT caught by thread\n", STDOUT_FILENO);
            pthread_cond_broadcast(&arrivedCond);
            pthread_mutex_unlock(&critMutex);

            return NULL;
        }
    }

    if(sigintFlag==1)
    {
        writeOutput("SIGINT caught by thread\n", STDOUT_FILENO);
        pthread_cond_broadcast(&arrivedCond);
        pthread_mutex_unlock(&critMutex);
        return NULL;
    }

    pthread_cond_broadcast(&arrivedCond);
    pthread_mutex_unlock(&critMutex);

    if(sigintFlag==1)
    {
        writeOutput("SIGINT caught by thread\n", STDOUT_FILENO);
        return NULL;
    }

    advancingOutput(output, dataSet->threadNo);
    if(writeOutput(output, STDOUT_FILENO)==-1)
        perror("Cannot write the advancing output of the thread");

    //Calculating the Discrete Fourier Transform of C
    calculateTransform(dataSet->startColumn, dataSet->endColumn);

    if(sigintFlag==1)
    {
        writeOutput("SIGINT caught by thread\n", STDOUT_FILENO);
        return NULL;
    }

    threadFinishOutput(output, dataSet->threadNo, threadStart);
    if(writeOutput(output, STDOUT_FILENO)==-1)
        perror("Cannot write the finish output of the thread");

    return NULL;
}

void calculateCColumns(int startIndex, int endIndex)
{
    for(int i=startIndex; i<endIndex; ++i)
    {
        for(int j=0; j<size; ++j)
        {
            int sum=0;
            for(int k=0; k<size; ++k)
            {
                sum+=matrixA[j][k]*matrixB[k][i];
            }

            matrixC[j][i]=sum;

            if(sigintFlag==1)
                return;
        }
    }
}

void calculateTransform(int start, int end)
{
    for(int i=0; i<size; i++)
    {
        for(int j=start; j<end; j++)
        {
            double reelPart=0;
            double imPart=0;
            for(int k=0; k<size; k++)
            {
                for(int m=0; m<size; m++)
                {
                    double x=(-2.0*PI*(double)i*(double)k/(double)size);
                    double y=(-2.0*PI*(double)j*(double)m/(double)size);
                    reelPart+=(matrixC[k][m]*1.0*cos(x+y));
                    imPart+=(matrixC[k][m]*1.0*sin(x+y));
                    if(sigintFlag==1)
                        return;
                }
            }
            res.reelMatrix[i][j]=reelPart;
            res.imMatrix[i][j]=imPart;
        }
    }
}

void arrangeStartOutput(char* output)
{
    time_t currentTime;
    time(&currentTime);

    struct tm *lt;
    lt=localtime(&currentTime);

    char res[1000];
    char buffer[1000];

    strftime(res, 1000, default_format, lt);
    strcpy(output, res);

    snprintf(buffer, 1000, ": Two matrices of size %dx%d have been read. The number of threads is %d\n", size, size, mCount);
    strcat(output, buffer);
}

void randezvousOutput(char* output, int no, struct timespec start)
{
    time_t currentTime;
    time(&currentTime);

    struct tm *lt;
    lt=localtime(&currentTime);

    char res[1000];
    char buffer[1000];

    strftime(res, 1000, default_format, lt);
    strcpy(output, res);

    struct timespec end;
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    double duration=(((end.tv_sec - start.tv_sec) * 1.0e3) + ((end.tv_nsec - start.tv_nsec) / 1.0e6))/1000.0;

    snprintf(buffer, 1000, ": Thread %d has reached the rendezvous point in %0.6f seconds.\n", no, duration);
    strcat(output, buffer);
}

void advancingOutput(char* output, int no)
{
    time_t currentTime;
    time(&currentTime);

    struct tm *lt;
    lt=localtime(&currentTime);

    char res[1000];
    char buffer[1000];

    strftime(res, 1000, default_format, lt);
    strcpy(output, res);

    snprintf(buffer, 1000, ": Thread %d is advancing to the second part\n", no);
    strcat(output, buffer);
}

void threadFinishOutput(char* output, int no, struct timespec start)
{
    time_t currentTime;
    time(&currentTime);

    struct tm *lt;
    lt=localtime(&currentTime);

    char res[1000];
    char buffer[1000];

    strftime(res, 1000, default_format, lt);
    strcpy(output, res);

    struct timespec end;
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    double duration=(((end.tv_sec - start.tv_sec) * 1.0e3) + ((end.tv_nsec - start.tv_nsec) / 1.0e6))/1000.0;

    snprintf(buffer, 1000, ": Thread %d has has finished the second part in %0.6f seconds.\n", no, duration);
    strcat(output, buffer);
}

void processFinishOutput(char* output, struct timespec start)
{
    time_t currentTime;
    time(&currentTime);

    struct tm *lt;
    lt=localtime(&currentTime);

    char res[1000];
    char buffer[1000];

    strftime(res, 1000, default_format, lt);
    strcpy(output, res);

    struct timespec end;
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    double duration=(((end.tv_sec - start.tv_sec) * 1.0e3) + ((end.tv_nsec - start.tv_nsec) / 1.0e6))/1000.0;

    snprintf(buffer, 1000, ": The process has written the output file. The total time spent is %0.6f seconds.\n", duration);
    strcat(output, buffer);
}

char* arrangeOutput(char** output)
{
    char buffer[1000];
    int allocationSize=0;

    for(int i=0; i<size; ++i)
        for(int j=0; j<size; ++j)
        {
            if(sigintFlag==1)
                return NULL;

            if(j!=size-1)
                snprintf(buffer, 1000, "%0.5f+(%0.5fi), ", res.reelMatrix[i][j], res.imMatrix[i][j]);
            else
                snprintf(buffer, 1000, "%0.5f+(%0.5fi)\n", res.reelMatrix[i][j], res.imMatrix[i][j]);

            allocationSize+=strlen(buffer);
        }
    *output=(char*)malloc(allocationSize+1);
    if(*output==NULL)
        return NULL;

    for(int i=0; i<size; ++i)
        for(int j=0; j<size; ++j)
        {
            if(sigintFlag==1)
                return NULL;

            if(j!=size-1)
                snprintf(buffer, 1000, "%0.5f+(%0.5fi), ", res.reelMatrix[i][j], res.imMatrix[i][j]);
            else
                snprintf(buffer, 1000, "%0.5f+(%0.5fi)\n", res.reelMatrix[i][j], res.imMatrix[i][j]);

            if(i==0 && j==0)
                strcpy(*output, buffer);
            else
                strcat(*output, buffer);
        }

    (*output)[allocationSize-1]='\0';
    return *output;
}

void sigintHandler(int signum)
{
    sigintFlag=1;
}
