#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>
#include <math.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

int sigintFlag=0;

float calcVariance(int* array, int size);
float calcCovariance(int* array1, int* array2, int size);
void formCovMatrix(char array[9][10], int* xVals, int* yVals, int* zVals);
void sigintHandler(int signum);

int main(int argc, char* argv[], char* const envp[])
{
    //Assigning handlers
    struct sigaction sigintAct={0};
    sigemptyset(&sigintAct.sa_mask);
    sigintAct.sa_handler=&sigintHandler;
    if(sigaction(SIGINT, &sigintAct, NULL)==-1)
    {
        perror("Cannot assign the signal handler");
        exit(-1);
    }

    //Initializing the x y z values
    int allX[10]={
        atoi(getenv("zeroth0")), atoi(getenv("first0")), atoi(getenv("second0")), atoi(getenv("third0")), atoi(getenv("fourth0")),
        atoi(getenv("fifth0")), atoi(getenv("sixth0")), atoi(getenv("seventh0")), atoi(getenv("eight0")), atoi(getenv("ninth0"))
    };
    int allY[10]={
        atoi(getenv("zeroth1")), atoi(getenv("first1")), atoi(getenv("second1")), atoi(getenv("third1")), atoi(getenv("fourth1")),
        atoi(getenv("fifth1")), atoi(getenv("sixth1")), atoi(getenv("seventh1")), atoi(getenv("eight1")), atoi(getenv("ninth1"))
    };
    int allZ[10]={
        atoi(getenv("zeroth2")), atoi(getenv("first2")), atoi(getenv("second2")), atoi(getenv("third2")), atoi(getenv("fourth2")),
        atoi(getenv("fifth2")), atoi(getenv("sixth2")), atoi(getenv("seventh2")), atoi(getenv("eight2")), atoi(getenv("ninth2"))
    };

    //Finding covariance matrix
    char covMatrix[9][10];
    formCovMatrix(covMatrix, allX, allY, allZ);

    //Opening the output file
    int outputFile=open(argv[1], O_WRONLY | O_APPEND, 0333);
    if(outputFile<0)
    {
        perror("The output file cannot be opened");
        exit(-1);
    }

    //Locking the output file for other child processes
    struct flock lock;
    memset(&lock, 0, sizeof(lock));
    lock.l_type=F_WRLCK;
    while(((fcntl(outputFile, F_SETLKW, &lock))==-1) && errno==EINTR);

    //Writing to the output file
    int memberCount;
    char* currentOutput;
    int byteswritten;

    if(sigintFlag==1)
    {
        printf("SIGINT caught by: %d\n", getpid());
        lock.l_type=F_UNLCK;
        while(((fcntl(outputFile, F_SETLKW, &lock))==-1) && errno==EINTR);
        close(outputFile);
        return 0;
    }

    for(memberCount=0; memberCount<9; ++memberCount)
    {
        if(memberCount==0 || memberCount==3 || memberCount==6)
        {
            currentOutput=(char*)malloc(strlen(covMatrix[memberCount])+3);
            memcpy(currentOutput, "(", 1);
            memcpy(currentOutput+1, covMatrix[memberCount], strlen(covMatrix[memberCount]));
            memcpy(currentOutput+1+strlen(covMatrix[memberCount]), " ", 1);
            currentOutput[(1+strlen(covMatrix[memberCount])+1)]='\0';
        }
        else if(memberCount==2 || memberCount==5)
        {
            currentOutput=(char*)malloc(strlen(covMatrix[memberCount])+3);
            memcpy(currentOutput, covMatrix[memberCount], strlen(covMatrix[memberCount]));
            memcpy(currentOutput+strlen(covMatrix[memberCount]), ") ", 2);
            currentOutput[1+strlen(covMatrix[memberCount])+1]='\0';
        }
        else if(memberCount==8)
        {
            currentOutput=(char*)malloc(strlen(covMatrix[memberCount])+3);
            memcpy(currentOutput, covMatrix[memberCount], strlen(covMatrix[memberCount]));
            memcpy(currentOutput+strlen(covMatrix[memberCount]), ")\n", 2);
            currentOutput[1+strlen(covMatrix[memberCount])+1]='\0';
        }
        else
        {
            currentOutput=(char*)malloc(strlen(covMatrix[memberCount])+2);
            memcpy(currentOutput, covMatrix[memberCount], strlen(covMatrix[memberCount]));
            memcpy(currentOutput+strlen(covMatrix[memberCount]), " ", 1);
            currentOutput[1+strlen(covMatrix[memberCount])+1]='\0';
        }

        while(((byteswritten=write(outputFile, currentOutput, strlen(currentOutput)))==-1) && (errno==EINTR));
        if(byteswritten<0)
        {
            perror("Cannot write to the file");
            lock.l_type=F_UNLCK;
            while(((fcntl(outputFile, F_SETLKW, &lock))==-1) && errno==EINTR);
            close(outputFile);
            free(currentOutput);
            exit(-1);
        }

        if(sigintFlag==1)
        {
            printf("SIGINT caught by: %d\n", getpid());
            lock.l_type=F_UNLCK;
            while(((fcntl(outputFile, F_SETLKW, &lock))==-1) && errno==EINTR);
            close(outputFile);
            free(currentOutput);
            return 0;
        }

        free(currentOutput);
    }

    //Unlocking the output file
    lock.l_type=F_UNLCK;
    while(((fcntl(outputFile, F_SETLKW, &lock))==-1) && errno==EINTR);

    close(outputFile);

    if(sigintFlag==1)
    {
        printf("SIGINT caught by: %d\n", getpid());
        close(outputFile);
        return 0;
    }

    exit(EXIT_SUCCESS);
}

float calcVariance(int* array, int size)
{
    //Finding the sum of all members
    int sum=0;
    for(int i=0; i<size; ++i)
        sum+=array[i];

    //Finding the mean
    float mean=((float)sum)/size;

    //Finding the variance
    float sumOfDif=0;
    for(int i=0; i<size; ++i)
        sumOfDif+=pow(((float)array[i])-mean, 2);

    return (sumOfDif/(size));
}

float calcCovariance(int* array1, int* array2, int size)
{
    //Finding the sum of all members
    int sum1=0;
    int sum2=0;
    for(int i=0; i<size; ++i)
    {
        sum1+=array1[i];
        sum2+=array2[i];
    }

    //Finding the mean
    float mean1=((float)sum1)/size;
    float mean2=((float)sum2)/size;

    //Finding the covariance
    float dif1[10];
    float dif2[10];
    float multpRes[10];
    float sumRes=0;
    for(int i=0; i<size; ++i)
    {
        dif1[i]=((float)array1[i])-mean1;
        dif2[i]=((float)array2[i])-mean2;
        multpRes[i]=(dif1[i]*dif2[i]);
        sumRes+=multpRes[i];
    }
    return (sumRes/((float)(size)));
}

void formCovMatrix(char array[9][10], int* xVals, int* yVals, int* zVals)
{
    //Calculating individual variances
    float varianceX=calcVariance(xVals, 10);
    float varianceY=calcVariance(yVals, 10);
    float varianceZ=calcVariance(zVals, 10);

    //Calculating covariances
    float covarianceXY=calcCovariance(xVals, yVals, 10);
    float covarianceXZ=calcCovariance(xVals, zVals, 10);
    float covarianceYZ=calcCovariance(yVals, zVals, 10);

    //Creating covariance matrix
    sprintf(array[0], "%.4f", varianceX);
    sprintf(array[1], "%.4f", covarianceXY);
    sprintf(array[2], "%.4f", covarianceXZ);
    sprintf(array[3], "%.4f", covarianceXY);
    sprintf(array[4], "%.4f", varianceY);
    sprintf(array[5], "%.4f", covarianceYZ);
    sprintf(array[6], "%.4f", covarianceXZ);
    sprintf(array[7], "%.4f", covarianceYZ);
    sprintf(array[8], "%.4f", varianceZ);
}

void sigintHandler(int signum)
{
    sigintFlag=1;
}

