#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

int sigintFlag=0;
int sigpipeFlag=0;
static const char default_format[] = "%b %d %Z %Y %H %M %S";

void sigintHandler(int signum);
void sigpipeHandler(int signum);

void checkArgc(int count);
int retrieveArguments(int count, char* arguements[], char** fifoPath, char** dataPath);
int createFifo(char* fifoPath);
int openFile(char* filePath, int flags, mode_t mode);
char* addDigit(char newDigit, char** oldNumber, int* oldNumberSize);
int* addNumber(int newNumber, int** numberArray, int* oldArraySize);
void freePointer(void* pointer);
struct timespec arrangeOutput1(char* output, char* fileName, int size);
char* arrangeInput(int size, int numbersMatrix[size][size], char* input);
int numPlaces(int n);
void arrangeOutput2(char* output, struct timespec start, char* result);

int main(int argc, char* argv[])
{
    setenv("TZ", "Europe/Istanbul", 1);
    tzset();

    //Checking command line arguments
    checkArgc(argc);

    //Retrieving the command line arguments & checking validity
    char* fifoFileName=NULL;
    char* dataFileName=NULL;
    if(retrieveArguments(argc, argv, &fifoFileName, &dataFileName)==-1)
    {
        char errorMessage[]="There is an invalid command line argument. You can only use \"-s\" and \"-o\". Please try again.\n";
        while((write(STDERR_FILENO, errorMessage, sizeof(errorMessage)-1)==-1) && (errno==EINTR));
        exit(EXIT_FAILURE);
    }

    //Checking file path validity
    if(fifoFileName==NULL || dataFileName==NULL)
    {
        char errorMessage[]="FIFO filepath or data filepath is not provided properly. Please try again.\n";
        while((write(STDERR_FILENO, errorMessage, sizeof(errorMessage)-1)==-1) && (errno==EINTR));
        exit(EXIT_FAILURE);
    }

    //Assigning signal handlers
    //SIGINT
    struct sigaction sigintAct={0};
    sigemptyset(&sigintAct.sa_mask);
    sigintAct.sa_handler=&sigintHandler;
    sigaddset(&sigintAct.sa_mask, SIGPIPE);
    if(sigaction(SIGINT, &sigintAct, NULL)==-1)
    {
        perror("Cannot assign the signal handler");
        exit(EXIT_FAILURE);
    }

    //Assigning signal handlers
    //SIGPIPE
    struct sigaction sigpipeAct={0};
    sigemptyset(&sigpipeAct.sa_mask);
    sigpipeAct.sa_handler=&sigpipeHandler;
    sigaddset(&sigpipeAct.sa_mask, SIGINT);
    if(sigaction(SIGPIPE, &sigpipeAct, NULL)==-1)
    {
        perror("Cannot assign the signal handler");
        exit(EXIT_FAILURE);
    }

    if(sigintFlag==1)
    {
        char message[]="SIGINT has been caught.\n";
        while((write(STDOUT_FILENO, message, sizeof(message)-1)==-1) && (errno==EINTR));
        return 0;
    }

    //Creating the client fifo if it does not exist
    char clientFifoPath[30];
    sprintf(clientFifoPath, "/tmp/%d", getpid());
    strcat(clientFifoPath, "FIFO.fifo");
    if(createFifo(clientFifoPath)==-1)
    {
        perror("FIFO cannot be created");
        exit(EXIT_FAILURE);
    }

    //Opening the data file
    int dataFile=openFile(dataFileName, O_RDONLY, 0555);
    if(dataFile<0)
    {
        perror("The data file cannot be opened");
        unlink(clientFifoPath);
        exit(EXIT_FAILURE);
    }

    if(sigintFlag==1)
    {
        char message[]="SIGINT has been caught.\n";
        while((write(STDOUT_FILENO, message, sizeof(message)-1)==-1) && (errno==EINTR));
        unlink(clientFifoPath);
        close(dataFile);
        return 0;
    }

    //Retrieving data
    int* tempNumbers=NULL;
    int tempSize=0;
    int row=0;
    int column=0;
    int aimedColumn=0;
    int currentDigit=0;
    char* readNumber=NULL;
    int counter2=0;
    while(true)
    {
        char readChar[1];
        int bytesread;
        while(((bytesread=read(dataFile, readChar, 1))==-1) && (errno==EINTR));
        if(bytesread<0)
        {
            perror("Cannot read from the file");
            freePointer(readNumber);
            freePointer(tempNumbers);
            unlink(clientFifoPath);
            close(dataFile);
            exit(EXIT_FAILURE);
        }

        if(sigintFlag==1)
        {
            char message[]="SIGINT has been caught.\n";
            while((write(STDOUT_FILENO, message, sizeof(message)-1)==-1) && (errno==EINTR));
            freePointer(readNumber);
            freePointer(tempNumbers);
            unlink(clientFifoPath);
            close(dataFile);
            return 0;
        }

        if(bytesread==0 && row==aimedColumn)
            break;
        else if(bytesread==0)
        {
            char errorMessage[]="Here: The data matrix should be square.\n";
            while((write(STDERR_FILENO, errorMessage, sizeof(errorMessage)-1)==-1) && (errno==EINTR));
            freePointer(readNumber);
            freePointer(tempNumbers);
            unlink(clientFifoPath);
            close(dataFile);
            exit(EXIT_FAILURE);
        }
        else if(readChar[0]=='\n')
        {
            if(currentDigit!=0)
            {
                tempNumbers=addNumber(atoi(readNumber), &tempNumbers, &tempSize);
                if(tempNumbers==NULL)
                {
                    perror("Cannot allocate memory");
                    freePointer(readNumber);
                    freePointer(tempNumbers);
                    unlink(clientFifoPath);
                    close(dataFile);
                    exit(-1);
                }
                counter2++;
                column++;
                currentDigit=0;

                free(readNumber);
                readNumber=NULL;

                if(aimedColumn!=0 && aimedColumn!=column)
                {
                    char errorMessage[]="The data matrix should be square.\n";
                    while((write(STDERR_FILENO, errorMessage, sizeof(errorMessage)-1)==-1) && (errno==EINTR));
                    freePointer(readNumber);
                    freePointer(tempNumbers);
                    unlink(clientFifoPath);
                    close(dataFile);
                    exit(-1);
                }
            }

            if(aimedColumn==0)
            {
                aimedColumn=column;
                if(aimedColumn<2)
                {
                    char errorMessage[]="The data matrix should have at least 2 columns.\n";
                    while((write(STDERR_FILENO, errorMessage, sizeof(errorMessage)-1)==-1) && (errno==EINTR));
                    freePointer(readNumber);
                    freePointer(tempNumbers);
                    unlink(clientFifoPath);
                    close(dataFile);
                    exit(EXIT_FAILURE);
                }
            }

            row++;
            column=0;
        }
        else if(readChar[0]!=' ' && readChar[0]!=',' && readChar[0]!='\t' && readChar[0]!='\r')
        {
            readNumber=addDigit(*readChar, &readNumber, &currentDigit);
            if(readNumber==NULL)
            {
                perror("Cannot allocate memory");
                freePointer(readNumber);
                freePointer(tempNumbers);
                unlink(clientFifoPath);
                close(dataFile);
                exit(EXIT_FAILURE);
            }
        }
        else if((readChar[0]==',') && currentDigit!=0)
        {
            tempNumbers=addNumber(atoi(readNumber), &tempNumbers, &tempSize);
            if(tempNumbers==NULL)
            {
                perror("Cannot allocate memory");
                freePointer(readNumber);
                freePointer(tempNumbers);
                unlink(clientFifoPath);
                close(dataFile);
                exit(EXIT_FAILURE);
            }
            counter2++;
            column++;
            currentDigit=0;

            free(readNumber);
            readNumber=NULL;
        }
    }

    if(counter2!=row*aimedColumn)
    {
        perror("The matrix should be square.\n");
        free(tempNumbers);
        unlink(clientFifoPath);
        close(dataFile);
        exit(EXIT_FAILURE);
    }

    int (*matrix)[aimedColumn]=malloc(sizeof(int)*row*aimedColumn);

    int counter=0;
    for(int i=0; i<row; ++i)
        for(int j=0; j<aimedColumn; ++j)
            matrix[i][j]=tempNumbers[counter++];

    free(tempNumbers);
    tempNumbers=NULL;

    if(sigintFlag==1)
    {
        char message[]="SIGINT has been caught.\n";
        while((write(STDOUT_FILENO, message, sizeof(message)-1)==-1) && (errno==EINTR));
        freePointer(matrix);
        unlink(clientFifoPath);
        close(dataFile);
        return 0;
    }

    //Opening the server fifo
    int fifoFile=openFile(fifoFileName, O_WRONLY, 0);
    if(fifoFile==-1)
    {
        perror("Cannot open server FIFO");
        freePointer(matrix);
        close(dataFile);
        unlink(clientFifoPath);
        exit(EXIT_FAILURE);
    }

    //Preparing server fifo input
    char* serverFifoInput=NULL;
    serverFifoInput=arrangeInput(row, matrix, serverFifoInput);
    if(serverFifoInput==NULL)
    {
        perror("Cannot allocate memory");
        free(matrix);
        close(dataFile);
        close(fifoFile);
        unlink(clientFifoPath);
        exit(EXIT_FAILURE);
    }

    if(sigintFlag==1)
    {
        char message[]="SIGINT has been caught.\n";
        while((write(STDOUT_FILENO, message, sizeof(message)-1)==-1) && (errno==EINTR));
        free(matrix);
        free(serverFifoInput);
        unlink(clientFifoPath);
        close(dataFile);
        close(fifoFile);
        return 0;
    }

    //Starting the connection
    char clientOutput[150];
    int byteswritten;
    struct timespec startTime=arrangeOutput1(clientOutput, dataFileName, row);
    while(((byteswritten=write(STDOUT_FILENO, clientOutput, strlen(clientOutput)))==-1) && (errno==EINTR));
    if(byteswritten<0)
    {
        perror("Cannot write to stdout");
        free(matrix);
        close(dataFile);
        close(fifoFile);
        unlink(clientFifoPath);
        free(serverFifoInput);
        exit(EXIT_FAILURE);
    }

    if(sigpipeFlag==1)
    {
        char message[]="SIGPIPE has been caught.\n";
        while((write(STDOUT_FILENO, message, sizeof(message)-1)==-1) && (errno==EINTR));
        free(matrix);
        free(serverFifoInput);
        unlink(clientFifoPath);
        close(dataFile);
        close(fifoFile);
        return 0;
    }

    //Writing to the server fifo
    while(((byteswritten=write(fifoFile, serverFifoInput, strlen(serverFifoInput)))==-1) && (errno==EINTR));
    if(byteswritten<0)
    {
        perror("Cannot write to the fifo file");
        free(matrix);
        close(dataFile);
        close(fifoFile);
        unlink(clientFifoPath);
        free(serverFifoInput);
        exit(EXIT_FAILURE);
    }

    if(sigpipeFlag==1)
    {
        char message[]="SIGPIPE has been caught.\n";
        while((write(STDOUT_FILENO, message, sizeof(message)-1)==-1) && (errno==EINTR));
        free(matrix);
        free(serverFifoInput);
        unlink(clientFifoPath);
        close(dataFile);
        close(fifoFile);
        return 0;
    }

    //Opening client fifo
    int clientFifoFile=openFile(clientFifoPath, O_RDONLY, 0);
    if(clientFifoFile==-1)
    {
        perror("Cannot open client FIFO");
        free(matrix);
        close(dataFile);
        close(fifoFile);
        unlink(clientFifoPath);
        free(serverFifoInput);
        exit(EXIT_FAILURE);
    }

    if(sigpipeFlag==1)
    {
        char message[]="SIGPIPE has been caught.\n";
        while((write(STDOUT_FILENO, message, sizeof(message)-1)==-1) && (errno==EINTR));
        free(matrix);
        free(serverFifoInput);
        unlink(clientFifoPath);
        close(dataFile);
        close(fifoFile);
        close(clientFifoFile);
        return 0;
    }

    //Receiving response
    char response[10]="Start";
    int bytesread;
    while(((bytesread=read(clientFifoFile, response, 10))==-1) && (errno==EINTR));
    if(bytesread<0)
    {
        perror("Cannot read the client fifo");
        free(serverFifoInput);
        free(matrix);
        close(dataFile);
        close(fifoFile);
        close(clientFifoFile);
        unlink(clientFifoPath);
        exit(EXIT_FAILURE);
    }

    if(sigpipeFlag==1)
    {
        char message[]="SIGPIPE has been caught.\n";
        while((write(STDOUT_FILENO, message, sizeof(message)-1)==-1) && (errno==EINTR));
        free(matrix);
        free(serverFifoInput);
        unlink(clientFifoPath);
        close(dataFile);
        close(fifoFile);
        close(clientFifoFile);
        return 0;
    }

    if(bytesread==0)
    {
        char message[]="No response received.\n";
        while((write(STDOUT_FILENO, message, sizeof(message)-1)==-1) && (errno==EINTR));
        free(serverFifoInput);
        free(matrix);
        close(dataFile);
        close(fifoFile);
        close(clientFifoFile);
        unlink(clientFifoPath);
        return 0;
    }

    if(sigintFlag==1)
    {
        char message[]="SIGINT has been caught.\n";
        while((write(STDOUT_FILENO, message, sizeof(message)-1)==-1) && (errno==EINTR));
        free(serverFifoInput);
        free(matrix);
        close(dataFile);
        close(fifoFile);
        close(clientFifoFile);
        unlink(clientFifoPath);
        return 0;
    }

    //Printing out the response
    arrangeOutput2(clientOutput, startTime, response);
    while(((byteswritten=write(STDOUT_FILENO, clientOutput, strlen(clientOutput)))==-1) && (errno==EINTR));
    if(byteswritten<0)
    {
        perror("Cannot write to stdout");
        free(serverFifoInput);
        free(matrix);
        close(dataFile);
        close(fifoFile);
        close(clientFifoFile);
        unlink(clientFifoPath);
        exit(EXIT_FAILURE);
    }

    free(serverFifoInput);
    free(matrix);
    close(dataFile);
    close(fifoFile);
    close(clientFifoFile);
    unlink(clientFifoPath);

    return 0;
}

void sigintHandler(int signum)
{
    sigintFlag=1;
}

void sigpipeHandler(int signum)
{
    sigpipeFlag=1;
}

void checkArgc(int count)
{
    if(count!=5)
    {
        char errorMessage[]="You need exactly 5 command line arguments (first being auto inputed) to run client. Please try again.\n";
        while((write(STDERR_FILENO, errorMessage, sizeof(errorMessage)-1)==-1) && (errno==EINTR));
        exit(EXIT_FAILURE);
    }
}

int retrieveArguments(int count, char* arguements[], char** fifoPath, char** dataPath)
{
    int option;
    while((option=getopt(count, arguements, "s:o:"))!=-1)
        switch(option)
        {
            case 's':
                *fifoPath=optarg;
                break;
            case 'o':
                *dataPath=optarg;
                break;
            default:
                return -1;
        }

    return 0;
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

int openFile(char* filePath, int flags, mode_t mode)
{
    int fd;
    if(mode!=0)
        while((fd=open(filePath, flags, mode))==-1 && errno==EINTR);
    else
        while((fd=open(filePath, flags))==-1 && errno==EINTR);

    return fd;
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

int* addNumber(int newNumber, int** numberArray, int* oldArraySize)
{
    if(*oldArraySize==0)
    {
        *numberArray=(int*)malloc(sizeof(int));
        if(numberArray==NULL)
            return NULL;
        (*numberArray)[0]=newNumber;
        (*oldArraySize)++;
        return *numberArray;
    }

    int* backupArray=(int*)malloc(sizeof(int)*(*oldArraySize));
    if(backupArray==NULL)
        return NULL;

    for(int i=0; i<*oldArraySize; ++i)
        backupArray[i]=(*numberArray)[i];

    *numberArray=(int*)realloc(*numberArray, (sizeof(int)*(*oldArraySize))+sizeof(int));
    if(*numberArray==NULL)
    {
        free(backupArray);
        return NULL;
    }

    for(int i=0; i<*oldArraySize; ++i)
        (*numberArray)[i]=backupArray[i];
    (*numberArray)[*oldArraySize]=newNumber;

    (*oldArraySize)++;

    free(backupArray);

    return *numberArray;
}

void freePointer(void* pointer)
{
    if(pointer!=NULL)
        free(pointer);
}

struct timespec arrangeOutput1(char* output, char* fileName, int size)
{
    char buffer[150];
    char res[50];

    struct tm *lt;
    time_t currentTime;
    (int)time(&currentTime);
    lt=localtime(&currentTime);

    struct timespec start;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

    strftime(res, 50, default_format, lt);
    strcpy(output, res);

    snprintf(buffer, 150, ": Client PID#%d: (", getpid());
    strcat(output, buffer);
    strcat(output, fileName);

    snprintf(buffer, 150, ") is submiting a %dx%d matrix.\n", size, size);
    strcat(output, buffer);

    return start;
}

char* arrangeInput(int size, int numbersMatrix[size][size], char* input)
{
    int allocationSize=(size*size)+1;
    allocationSize+=numPlaces(getpid())+1;
    allocationSize+=numPlaces(size)+1;
    for(int i=0; i<size; ++i)
        for(int j=0; j<size; ++j)
            allocationSize+=numPlaces(numbersMatrix[i][j]);

    input=(char*)malloc(allocationSize);
    if(input==NULL)
        return NULL;

    char buffer[allocationSize];
    snprintf(buffer, allocationSize, "%d ", getpid());
    strcpy(input, buffer);
    snprintf(buffer, allocationSize, "%d ", size);
    strcat(input, buffer);
    for(int i=0; i<size; ++i)
        for(int j=0; j<size; ++j)
        {
            if(j==size-1 && i==size-1)
                snprintf(buffer, allocationSize, "%d", numbersMatrix[i][j]);
            else
                snprintf(buffer, allocationSize, "%d ", numbersMatrix[i][j]);

            strcat(input, buffer);
        }

    strcat(input, "\n");
    input[allocationSize-1]='\0';

    return input;
}

int numPlaces (int n) {
    if (n == 0) return 1;
    int res=floor (log10 (abs (n))) + 1;
    if(n<0)
        res++;

    return res;
}

void arrangeOutput2(char* output, struct timespec start, char* result)
{
    time_t currentTime;
    (int)time(&currentTime);
    char buffer[150];

    struct timespec end;
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    double duration=(((end.tv_sec - start.tv_sec) * 1.0e3) + ((end.tv_nsec - start.tv_nsec) / 1.0e6))/1000.0;

    char res[50];
    struct tm *lt;

    lt=localtime(&currentTime);
    strftime(res, 50, default_format, lt);

    output[0]='\0';
    strcpy(output, res);

    strcat(output, ": Client PID#");

    snprintf(buffer, 150, "%d: the matrix is ", getpid());
    strcat(output, buffer);

    if(strcmp(result, "No")==0)
        strcat(output, "not ");

    strcat(output, "invertible, total time ");

    snprintf(buffer, 100, "%0.4f seconds, goodbye.\n", duration);
    strcat(output, buffer);
}
