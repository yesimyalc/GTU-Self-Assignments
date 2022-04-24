#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <math.h>

int inputFile;
int outputFile;
char* outputFileName;

int sigCaughtFlag=0;
int sigintFlag=0;

int* childrenIDs=NULL;
int childCounter=0;
int cleanedCounter=0;

void closeFiles();
void clean();

void sigintHandler(int signum);

void storeChildId(int id, int size);
char* addDigit(char newDigit, char* oldNumber, int* oldNumberSize);
void calcMinDistance(float array[childCounter][3][3]);
void createFrobNorm(float array[childCounter][3][3], float frobNormArray[childCounter]);

int main(int argc, char* argv[])
{
    //Checking command line argument count validity
    if(argc!=5)
    {
        fprintf(stderr, "This program needs exactly 5 command line inputs to function properly. Please try again.");
        exit(-1);
    }

    const char* inputFileName;

    if(strcmp(argv[1], "-i")==0 && strcmp(argv[3], "-o")==0)
    {
        inputFileName=argv[2];
        outputFileName=argv[4];
    }
    else if(strcmp(argv[1], "-o")==0 && strcmp(argv[3], "-i")==0)
    {
        inputFileName=argv[4];
        outputFileName=argv[2];
    }
    else
    {
        fprintf(stderr, "The command line arguments need to include \"-i\" and \"-o\" to mention the input/output file names. Please try again.");
        exit(-1);
    }

    //Opening the input file
    inputFile=open(inputFileName, O_RDONLY, 0555);
    if(inputFile<0)
    {
        perror("The input file cannot be opened");
        exit(-1);
    }

    //Opening the output file
    outputFile=open(outputFileName, O_RDONLY | O_TRUNC | O_CREAT, 0777);
    if(outputFile<0)
    {
        perror("The output file cannot be opened");
        close(inputFile);
        exit(-1);
    }

    //Assigning signal handlers
    //SIGINT
    struct sigaction sigintAct={0};
    sigemptyset(&sigintAct.sa_mask);
    sigintAct.sa_handler=&sigintHandler;
    if(sigaction(SIGINT, &sigintAct, NULL)==-1)
    {
        perror("Cannot assign the signal handler");
        clean();
        exit(-1);
    }

    //Reading data from the inputFile
    printf("Process P reading %s\n", inputFileName);
    while(true)
    {
        unsigned char readChar[1];
        int currentDigit=0;
        int bytesread;
        unsigned char coordinates[10][3];
        int numberCounter=0;
        int coordinateCounter;

        if(sigintFlag==1)
        {
            clean();
            printf("SIGINT is caught by the main process. Process will shut down.\n");
            return 0;
        }

        for(coordinateCounter=0; coordinateCounter<10;)
        {
            while(((bytesread=read(inputFile, readChar, 1))==-1) && (errno==EINTR));
            if(bytesread<0)
            {
                perror("Cannot read from the file");
                clean();
                exit(-1);
            }

            coordinates[coordinateCounter][numberCounter]=readChar[0];
            numberCounter++;
            if(numberCounter==3)
            {
                coordinateCounter++;
                numberCounter=0;
            }
            currentDigit=0;

            if(bytesread==0)
                break;
        }

        if(coordinateCounter==10)
        {
            if(sigintFlag==1)
            {
                clean();
                printf("SIGINT is caught by the main process. Process will shut down.\n");
                return 0;
            }

            childCounter++;
            int childId=fork();
            if(childId==-1)
            {
                perror("Fork failed");
                clean();
                exit(1);
            }
            else if(childId==0)
            {
                const char* fileName="./processR_i";
                char* keys[]={
                    "zeroth0=", "zeroth1=", "zeroth2=", "first0=", "first1=", "first2=",
                    "second0=", "second1=", "second2=", "third0=", "third1=", "third2=",
                    "fourth0=", "fourth1=", "fourth2=", "fifth0=", "fifth1=", "fifth2=",
                    "sixth0=", "sixth1=", "sixth2=", "seventh0=", "seventh1=", "seventh2=",
                    "eight0=", "eight1=", "eight2=", "ninth0=", "ninth1=", "ninth2="
                };
                char keyToValue[30][100];
                numberCounter=0;
                coordinateCounter=0;
                printf("Created R_%d with ",childCounter);
                for(int i=0; i<30; i++)
                {
                    char buffer[10];
                    strcpy(keyToValue[i], keys[i]);
                    sprintf(buffer, "%d", coordinates[coordinateCounter][numberCounter]);
                    strcat(keyToValue[i], buffer);
                    numberCounter++;
                    if(numberCounter==3)
                    {
                        numberCounter=0;
                        coordinateCounter++;
                        printf("%d)", coordinates[coordinateCounter][numberCounter-1]);

                        if(i!=29)
                            printf(", ");
                    }
                    else if(numberCounter==1)
                        printf("(%d, ", coordinates[coordinateCounter][numberCounter-1]);
                    else
                        printf("%d, ", coordinates[coordinateCounter][numberCounter-1]);
                }
                printf("\n");
                char* argList[]={"./processR_i", outputFileName, 0};
                char* envList[]={
                    keyToValue[0], keyToValue[1], keyToValue[2], keyToValue[3], keyToValue[4], keyToValue[5],
                    keyToValue[6], keyToValue[7], keyToValue[8], keyToValue[9], keyToValue[10], keyToValue[11],
                    keyToValue[12], keyToValue[13], keyToValue[14], keyToValue[15], keyToValue[16], keyToValue[17],
                    keyToValue[18], keyToValue[19], keyToValue[20], keyToValue[21], keyToValue[22], keyToValue[23],
                    keyToValue[24], keyToValue[25], keyToValue[26], keyToValue[27], keyToValue[28], keyToValue[29],
                    0
                };

                if(sigintFlag==1)
                {
                    printf("SIGINT caught by: %d\n", getpid());
                    return 0;
                }

                execve(fileName, argList, envList);
                perror("Child process initiation failed");
                abort();
            }
            else
                storeChildId(childId, childCounter);
        }

        if(bytesread==0)
            break;

    }

    //Making sure all child processes are finished with their job
    for(int i=0; i<childCounter; ++i)
    {
        int pid=waitpid(-1, NULL, 0);
        cleanedCounter++;
        if(pid<=0)
        {
            fprintf(stderr, "Wait failed.");
            clean();
            exit(-1);
        }
    }

    if(sigintFlag==1)
    {
        printf("SIGINT is caught by the main process. Process will shut down.\n");
        clean();
        return 0;
    }

    printf("Reached EOF, collecting outputs from %s.\n", outputFileName);

    //Reading the output file
    float matrixes[childCounter][3][3];
    int lineNo=0;
    int currentDigit=0;
    int row=0;
    int column=0;
    char* readNumber=NULL;
    while(true)
    {
        char readChar[1];
        int bytesread;

        while(((bytesread=read(outputFile, readChar, 1))==-1) && (errno==EINTR));
        if(bytesread<0)
        {
            perror("Cannot read from the file");
            if(readNumber!=NULL)
                free(readNumber);
            clean();
            exit(-1);
        }

        if(bytesread==0)
            break;
        else if(readChar[0]=='\n' || readChar[0]=='\r')
        {
            lineNo++;
            row=0;
        }
        else if(readChar[0]!=' ' && readChar[0]!=')' && readChar[0]!='\t' && readChar[0]!='(')
            readNumber=addDigit(*readChar, readNumber, &currentDigit);
        else if((readChar[0]==' ' || readChar[0]==')' || readChar[0]=='\t') && currentDigit!=0)
        {
            matrixes[lineNo][row][column]=strtof(readNumber, NULL);
            column++;
            if(readChar[0]==')')
            {
                column=0;
                row++;
            }
            currentDigit=0;

            free(readNumber);
        }
    }

    //Result calculation
    calcMinDistance(matrixes);

    if(sigintFlag==1)
    {
        printf("SIGINT is caught by the main process. Process will shut down.\n");
        clean();
        return 0;
    }

    printf("All process are done\n");

    closeFiles();
    free(childrenIDs);

    return 0;
}

void closeFiles()
{
    close(inputFile);
    close(outputFile);
}

void storeChildId(int id, int size)
{
    if(size==1)
    {
        childrenIDs=(int*)malloc(sizeof(int));
        if(childrenIDs==NULL)
        {
            perror("Cannot allocate memory");
            clean();
            exit(-1);
        }
        childrenIDs[0]=id;
    }
    else
    {
        int* backupList=(int*)malloc((sizeof(int)*(size-1)));
        if(backupList==NULL)
        {
            perror("Cannot allocate memory");
            clean();
            exit(-1);
        }
        for(int i=0; i<size-1; ++i)
            backupList[i]=childrenIDs[i];

        childrenIDs=(int*)realloc(childrenIDs, (sizeof(int)*size));
        if(childrenIDs==NULL)
        {
            perror("Cannot allocate memory");
            free(backupList);
            clean();
            exit(-1);
        }
        for(int i=0; i<size-1; ++i)
            childrenIDs[i]=backupList[i];
        childrenIDs[size-1]=id;

        free(backupList);
    }
}

void sigintHandler(int signum)
{
    sigintFlag=1;
}

void clean()
{
    closeFiles();

    for(int i=0; i<childCounter; ++i)
        kill(childrenIDs[i], SIGINT);

    for(int i=0; i<childCounter-cleanedCounter; ++i)
    {
        int pid=waitpid(-1, NULL, 0);
        if(pid<=0)
        {
            fprintf(stderr, "Wait failed.");
            remove(outputFileName);
            if(childrenIDs!=NULL)
                free(childrenIDs);
            exit(-1);
        }
    }

    remove(outputFileName);
    if(childrenIDs!=NULL)
        free(childrenIDs);
}

char* addDigit(char newDigit, char* oldNumber, int* oldNumberSize)
{
    if(*oldNumberSize==0)
    {
        oldNumber=(char*)malloc(2);
        if(oldNumber==NULL)
        {
            perror("Cannot allocate memory");
            clean();
            exit(-1);
        }
        oldNumber[0]=newDigit;
        oldNumber[1]='\0';
        (*oldNumberSize)++;
        return oldNumber;
    }

    char* backupNumber=(char*)malloc(*oldNumberSize);
    if(backupNumber==NULL)
    {
        perror("Cannot allocate memory");
        clean();
        exit(-1);
    }
    for(int i=0; i<*oldNumberSize; ++i)
        backupNumber[i]=oldNumber[i];

    oldNumber=(char*)realloc(oldNumber, (*oldNumberSize)+2);
    if(oldNumber==NULL)
    {
        perror("Cannot allocate memory");
        free(backupNumber);
        clean();
        exit(-1);
    }
    for(int i=0; i<*oldNumberSize; ++i)
        oldNumber[i]=backupNumber[i];
    oldNumber[*oldNumberSize]=newDigit;
    oldNumber[*oldNumberSize+1]='\0';

    (*oldNumberSize)++;

    free(backupNumber);

    return oldNumber;
}

void calcMinDistance(float array[childCounter][3][3])
{
    float magnitudes[childCounter];
    createFrobNorm(array, magnitudes);
    int index1;
    int index2;
    float minDif=fabs(magnitudes[0]-magnitudes[1]);

    for(int i=0; i<childCounter; ++i)
        for(int j=i+1; j<childCounter; ++j)
        {
            float dif=fabs(magnitudes[i]-magnitudes[j]);
            if(dif<minDif)
            {
                index1=i;
                index2=j;
                minDif=dif;
            }
        }

    printf("The closest 2 matrices are ");
    for(int i=0; i<3; i++)
    {
        printf("(");
        for(int j=0; j<3; ++j)
        {
            if(j!=2)
                printf("%.4f, ", array[index1][i][j]);
            else
                printf("%.4f) ", array[index1][i][j]);
        }
    }
    printf(" and ");
    for(int i=0; i<3; i++)
    {
        printf("(");
        for(int j=0; j<3; ++j)
        {
            if(j!=2)
                printf("%.4f, ", array[index2][i][j]);
            else
                printf("%.4f) ", array[index2][i][j]);
        }
    }
    printf(", and their distance is %.4f\n", minDif);
}

void createFrobNorm(float array[childCounter][3][3], float frobNormArray[childCounter])
{
    for(int i=0; i<childCounter; ++i)
    {
        float sumOfSquares=0;
        for(int j=0; j<3; ++j)
            for(int k=0; k<3; ++k)
                sumOfSquares+=pow(array[i][j][k], 2);
        frobNormArray[i]=sqrt(sumOfSquares);
    }
}
