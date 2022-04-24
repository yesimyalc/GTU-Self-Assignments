#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/stat.h>
#include <ctype.h>

struct stringPairs{
    char string1[1000];
    char string2[1000];

    int repeatedCharIndex[1000];
    int repeatedCharCount;

    int changableCharIndex[1000]; //the indexes of the []
    int changableCharCount; //amount of []
    char changableChars[1000][94]; //chars

    bool caseInsensetive;
    bool lineStart;
    bool lineEnd;
};

int isInIntArray(int* array, int arraySize, int element);

int main(int argc, char* argv[])
{
    //Checking command line argument count validity
    if(argc!=3)
    {
        fprintf(stderr, "This program needs exactly 2 command line inputs to function properly. Please try again.");
        exit(-1);
    }

    //Checking command line argument validity
    int arg1Len=strlen(argv[1]);
    int arg2Len=strlen(argv[2]);

    if(strncmp(argv[1], "/", 1)!=0)
    {
        fprintf(stderr, "The first command line argument must start with the character \"/\". Please try again.");
        exit(-1);
    }
    else if(strncmp(argv[1]+arg1Len-1, "/", 1)!=0 && strncmp(argv[1]+arg1Len-2, "/i", 2)!=0)
    {
        fprintf(stderr, "The first command line argument must end with the characters \"/\" or \"/i\". Please try again.");
        exit(-1);
    }

    int slashCounter=0;
    int pairCounter=1;
    for(int i=0; i<arg1Len; i++)
    {
        if(strncmp(argv[1]+i,"/",1)==0)
            slashCounter+=1;
        else if(strncmp(argv[1]+i,";",1)==0)
        {
            if(slashCounter==3)
            {
                slashCounter=0;
                pairCounter+=1;
            }
            else if(slashCounter!=3 && strncmp(argv[1]+i+1,"/",1)==0 && strncmp(argv[1]+i-1,"/",1)==0)
            {
                fprintf(stderr, "Your first command line argument includes invalid amount of strings. Please try again.");
                exit(-1);
            }

            if(strncmp(argv[1]+i+1,"/",1)!=0 && slashCounter==0)
            {
                fprintf(stderr, "You need to have \"/\" after \";\". Please try again.");
                exit(-1);
            }
        }

        if(slashCounter>3)
        {
            fprintf(stderr, "Your first command line argument includes invalid amount of strings. Please try again.");
            exit(-1);
        }
    }
    if(slashCounter==2)
    {
        fprintf(stderr, "The command line argument does not include necessary amount of strings. Please try again.");
        exit(-1);
    }
    slashCounter=0;

    //Analyzing the string replacement requests
    struct stringPairs currentPairs[pairCounter];
    for(int i=0; i<pairCounter; ++i)
    {
        currentPairs[i].string1[0]='\0';
        currentPairs[i].string2[0]='\0';
        for(int j=0; j<1000; j++)
        {
            currentPairs[i].changableChars[j][0]='\0';
        }
        currentPairs[i].repeatedCharCount=0;
        currentPairs[i].changableCharCount=0;
        currentPairs[i].caseInsensetive=false;
        currentPairs[i].lineStart=false;
        currentPairs[i].lineEnd=false;
    }
    int currentPairNo=0;
    int currentCharIndex=-1;
    for(int i=0; i<arg1Len; ++i)
    {
        if(strncmp(argv[1]+i, "/", 1)==0)
            slashCounter+=1;
        else if(strncmp(argv[1]+i, ";", 1)==0)
        {
            slashCounter=0;
            currentPairNo+=1;
            currentCharIndex=-1;
        }
        else if(strncmp(argv[1]+i, "^", 1)==0)
        {
            if(slashCounter!=1)
            {
                fprintf(stderr, "You cannot have \"^\" in the string that you want to replace with. Please enter the strings again.");
                exit(-1);
            }
            else if(strncmp(argv[1]+i-1, "/", 1)!=0)
            {
                fprintf(stderr, "\"^\" can only be used after \"/\". Please enter the strings again.");
                exit(-1);
            }
            else if(strncmp(argv[1]+i+1, "/", 1)==0 || strncmp(argv[1]+i+1, "$", 1)==0)
            {
                fprintf(stderr, "\"^\" cannot be used right before \"/\" or \"$\". Please enter the strings again.");
                exit(-1);
            }
            else if(currentPairs[currentPairNo].lineEnd==true)
            {
                fprintf(stderr, "Searching both only at the end and at the start of the lines are not supported. Please try again.");
                exit(-1);
            }
            else
                currentPairs[currentPairNo].lineStart=true;
        }
        else if(strncmp(argv[1]+i, "$", 1)==0)
        {
            if(slashCounter!=1)
            {
                fprintf(stderr, "You cannot have \"$\" in the string that you want to replace with. Please enter the strings again.");
                exit(-1);
            }
            else if(strncmp(argv[1]+i+1, "/", 1)!=0)
            {
                fprintf(stderr, "\"$\" can only be used before \"/\". Please enter the strings again.");
                exit(-1);
            }
            else if(strncmp(argv[1]+i-1, "/", 1)==0 || strncmp(argv[1]+i-1, "^", 1)==0)
            {
                fprintf(stderr, "\"$\" can not be used right after \"/\" or \"^\". Please enter the strings again.");
                exit(-1);
            }
            else if(currentPairs[currentPairNo].lineStart==true)
            {
                fprintf(stderr, "Searching both only at the end and at the start of the lines are not supported. Please try again.");
                exit(-1);
            }
            else
                currentPairs[currentPairNo].lineEnd=true;
        }
        else if(strncmp(argv[1]+i, "i", 1)==0 && slashCounter==3)
            currentPairs[currentPairNo].caseInsensetive=true;
        else if(strncmp(argv[1]+i, "*", 1)==0)
        {
            if(slashCounter!=1)
            {
                fprintf(stderr, "You cannot have \"*\" in the string that you want to replace with. Please enter the strings again.");
                exit(-1);
            }
            else if(strncmp(argv[1]+i-1, "/", 1)==0 || strncmp(argv[1]+i-1, "*", 1)==0)
            {
                fprintf(stderr, "\"*\" cannot be used after \"/\" or \"*\". Please enter the strings again.");
                exit(-1);
            }
            else
            {
                currentPairs[currentPairNo].repeatedCharIndex[currentPairs[currentPairNo].repeatedCharCount]=currentCharIndex;
                currentPairs[currentPairNo].repeatedCharCount++;
            }
        }
        else if(strncmp(argv[1]+i, "[", 1)==0)
        {
            if(slashCounter!=1)
            {
                fprintf(stderr, "You cannot have \"[\" in the string that you want to replace with. Please enter the strings again.");
                exit(-1);
            }

            currentCharIndex++;
            currentPairs[currentPairNo].changableCharIndex[currentPairs[currentPairNo].changableCharCount]= currentCharIndex;
            currentPairs[currentPairNo].changableCharCount++;

            for(int j=i+1; j<arg1Len-1; ++j)
            {
                if(strncmp(argv[1]+j, "]", 1)==0 && ((j==i+1) || (j==i+2)))
                {
                    fprintf(stderr, "[] must contain at least 2 characters inside. Please enter the string inputs again.");
                    exit(-1);
                }
                else if(strncmp(argv[1]+j, "]", 1)==0)
                {
                    i=j;
                    break;
                }
                else if(strncmp(argv[1]+j, "]", 1)!=0 && j==arg1Len-3)
                {
                    fprintf(stderr, "\"[\" must be followed with a \"]\". There is no \"]\" detected. Please try again.");
                    exit(-1);
                }
                else if(strncmp(argv[1]+j, "*", 1)!=0 && strncmp(argv[1]+j, "^", 1)!=0 && strncmp(argv[1]+j, "[", 1)!=0 && strncmp(argv[1]+j, "$", 1)!=0 && strncmp(argv[1]+j, "[", 1)!=0 && strncmp(argv[1]+j, "/", 1)!=0)
                {
                    strncat(currentPairs[currentPairNo].changableChars[currentPairs[currentPairNo].changableCharCount-1], argv[1]+j, 1);
                    if(j==i+1)
                        strncat(currentPairs[currentPairNo].string1, argv[1]+j, 1);
                }
                else
                {
                    fprintf(stderr, "There is an invalid character detected in \"[]\". %c Cannot be used here. Please try again.", *(argv[1]+j));
                    exit(-1);
                }
            }
        }
        else if(strncmp(argv[1]+i, "]", 1)==0)
        {
            fprintf(stderr, "The usage of \"]\" is wrong. It can be used after a \"[\" and cannot be used in the string that you want to replace with. Please enter the strings again.");
            exit(-1);
        }
        else
        {
            if(slashCounter==1)
                strncat(currentPairs[currentPairNo].string1, argv[1]+i, 1);
            else
                strncat(currentPairs[currentPairNo].string2, argv[1]+i, 1);

            currentCharIndex++;
        }
    }
    currentPairNo=0;

    //Opening the files
    int inputFile=open(argv[2], O_RDWR, 0777);
    if(inputFile<0)
    {
        perror("The input file cannot be opened");
        exit(-1);
    }

    char tempName[20];
    strcpy(tempName,"/tmp/fileXXXXXX");
    int tempFile=mkstemp(tempName);
    if(tempFile<0)
    {
        perror("The temporary file for output cannot be opened");
        close(inputFile);
        exit(-1);
    }

    //Locking the file
    struct flock lock;
    memset(&lock, 0, sizeof(lock));
    lock.l_type=F_WRLCK;
    int res=fcntl(inputFile, F_SETLK, &lock);
    if(res==-1)
    {
	perror("Cannot lock the file currently");
	close(inputFile);
	close(tempFile);
	unlink(tempName);
	exit(-1);
    }

    //String matching & replacing
    char buffer[1000];
    char readChar[100];
    char tempBuffer[1000];
    char backupBuffer[1000];
    int bytesread;
    int byteswritten;
    char following[1];
    char* foundSubstr;
    int bufferLength=0;
    bool isLineStart=false;
    bool isLineEnd=false;

    buffer[0]='\0';
    tempBuffer[0]='\0';
    backupBuffer[0]='\0';
    following[0]='\0';


    while(true)
    {
        while(true)
        {
            while(((bytesread=read(inputFile, readChar, 1))==-1) && (errno==EINTR));
            if(bytesread<0)
            {
                perror("Cannot read from the file");
                close(inputFile);
                close(tempFile);
                exit(-1);
            }

            if((readChar[0]=='\n') || (readChar[0]=='\r') || (readChar[0]==' ') || (readChar[0]=='\t'))
            {
                following[0]=readChar[0];
                if(following[0]=='\n' || following[0]=='\r')
                    isLineEnd=true;

                break;
            }
            else if(bytesread==0)
                break;
            else
            {
                strncat(buffer, readChar, 1);
                bufferLength++;
            }
        }

        for(int i=0; i<pairCounter; ++i)
        {
            if(currentPairs[i].caseInsensetive==true)
            {
                for(int j = 0; currentPairs[i].string1[j]; j++)
                    currentPairs[i].string1[j] = tolower(currentPairs[i].string1[j]);

                strcpy(tempBuffer, buffer);
                for(int j = 0; tempBuffer[j]; j++)
                    tempBuffer[j] = tolower(tempBuffer[j]);
            }

            if(!((currentPairs[i].lineStart==true && isLineStart==false) || (currentPairs[i].lineEnd==true && isLineEnd==false)) )
            {
                backupBuffer[0]='\0';
                strcat(backupBuffer, buffer);
                char comparisonBuffer[1000];
                comparisonBuffer[0]='\0';
                if(currentPairs[i].caseInsensetive==true)
                    strcpy(comparisonBuffer, tempBuffer);
                else
                    strcpy(comparisonBuffer, buffer);

                foundSubstr=strstr(comparisonBuffer, currentPairs[i].string1);

                if(currentPairs[i].repeatedCharCount!=0 && currentPairs[i].changableCharCount==0)
                {
                    int j;
                    int matchIndex=-1;
                    int startIndex=-1;
                    int extraCount=0;
                    int stack=0;

                    for(j=0; j<strlen(backupBuffer); j++)
                    {
                        if(matchIndex+1==strlen(currentPairs[i].string1))
                        {
                            strcpy(buffer+startIndex-stack,currentPairs[i].string2);
                            strcpy(buffer+startIndex+strlen(currentPairs[i].string2)-stack, backupBuffer+startIndex+strlen(currentPairs[i].string1)+extraCount);
                            bufferLength+=strlen(currentPairs[i].string2)-strlen(currentPairs[i].string1)-extraCount;
                            stack+=strlen(currentPairs[i].string1)-strlen(currentPairs[i].string2)+extraCount;

                            matchIndex=-1;
                            startIndex=-1;
                            extraCount=0;
                        }
                        else if(comparisonBuffer[j]==currentPairs[i].string1[matchIndex+1] && isInIntArray(currentPairs[i].repeatedCharIndex, currentPairs[i].repeatedCharCount, matchIndex+1)!=-1)
                        {
                            matchIndex++;
                            if(startIndex==-1)
                                startIndex=j;

                            while(j<strlen(backupBuffer)-1)
                            {
                                if(comparisonBuffer[j+1]==currentPairs[i].string1[matchIndex])
                                {
                                    j++;
                                    extraCount++;
                                }
                                else
                                    break;
                            }

                            if(matchIndex+1==strlen(currentPairs[i].string1))
                            {
                                strcpy(buffer+startIndex-stack,currentPairs[i].string2);
                                strcpy(buffer+startIndex+strlen(currentPairs[i].string2)-stack, backupBuffer+startIndex+strlen(currentPairs[i].string1)+extraCount);
                                bufferLength+=strlen(currentPairs[i].string2)-strlen(currentPairs[i].string1)-extraCount;
                                stack+=strlen(currentPairs[i].string1)-strlen(currentPairs[i].string2)+extraCount;

                                matchIndex=-1;
                                startIndex=-1;
                                extraCount=0;
                            }
                        }
                        else if(comparisonBuffer[j]==currentPairs[i].string1[matchIndex+1])
                        {
                            matchIndex++;
                            if(startIndex==-1)
                                startIndex=j;

                            if(matchIndex+1==strlen(currentPairs[i].string1))
                            {
                                strcpy(buffer+startIndex-stack,currentPairs[i].string2);
                                strcpy(buffer+startIndex+strlen(currentPairs[i].string2)-stack, backupBuffer+startIndex+strlen(currentPairs[i].string1)+extraCount);
                                bufferLength+=strlen(currentPairs[i].string2)-strlen(currentPairs[i].string1)-extraCount;
                                stack+=strlen(currentPairs[i].string1)-strlen(currentPairs[i].string2)+extraCount;

                                matchIndex=-1;
                                startIndex=-1;
                                extraCount=0;
                            }
                        }
                        else if(comparisonBuffer[j]!=currentPairs[i].string1[matchIndex+1] && isInIntArray(currentPairs[i].repeatedCharIndex, currentPairs[i].repeatedCharCount, matchIndex+1)!=-1)
                        {
                            matchIndex++;
                            j--;
                            extraCount--;
                        }
                        else if(matchIndex!=-1 && comparisonBuffer[j]!=currentPairs[i].string1[matchIndex+1])
                        {
                            if(startIndex!=-1)
                                j--;
                            matchIndex=-1;
                            startIndex=-1;
                            extraCount=0;

                            if(matchIndex+1==strlen(currentPairs[i].string1))
                            {
                                strcpy(buffer+startIndex-stack,currentPairs[i].string2);
                                strcpy(buffer+startIndex+strlen(currentPairs[i].string2)-stack, backupBuffer+startIndex+strlen(currentPairs[i].string1)+extraCount);
                                bufferLength+=strlen(currentPairs[i].string2)-strlen(currentPairs[i].string1)-extraCount;
                                stack+=strlen(currentPairs[i].string1)-strlen(currentPairs[i].string2)+extraCount;

                                matchIndex=-1;
                                startIndex=-1;
                                extraCount=0;
                            }
                        }

                        if(j==strlen(backupBuffer)-1 && isInIntArray(currentPairs[i].repeatedCharIndex, currentPairs[i].repeatedCharCount, matchIndex+1)!=-1 && matchIndex!=-1)
                        {
                            for(int k=strlen(backupBuffer)-extraCount-startIndex; k<strlen(currentPairs[i].string1); k++)
                            {
                                if(isInIntArray(currentPairs[i].repeatedCharIndex, currentPairs[i].repeatedCharCount, k)==-1)
                                    break;
                                else
                                    extraCount--;

                                if(k==strlen(currentPairs[i].string1)-1)
                                {
                                    strcpy(buffer+startIndex-stack,currentPairs[i].string2);
                                    strcpy(buffer+startIndex+strlen(currentPairs[i].string2)-stack, backupBuffer+startIndex+strlen(currentPairs[i].string1)+extraCount);
                                    bufferLength+=strlen(currentPairs[i].string2)-strlen(currentPairs[i].string1)-extraCount;
                                    break;
                                }
                            }
                        }
                    }
                }
                else if(currentPairs[i].repeatedCharCount==0 && currentPairs[i].changableCharCount!=0)
                {
                    int j;
                    int matchIndex=-1;
                    int startIndex=-1;
                    int stack=0;

                    for(j=0; j<strlen(backupBuffer); j++)
                    {
                        int changableIndex=isInIntArray(currentPairs[i].changableCharIndex, currentPairs[i].changableCharCount, matchIndex+1);

                        if(matchIndex+1==strlen(currentPairs[i].string1))
                        {
                            strcpy(buffer+startIndex-stack,currentPairs[i].string2);
                            strcpy(buffer+startIndex+strlen(currentPairs[i].string2)-stack, backupBuffer+startIndex+strlen(currentPairs[i].string1));
                            bufferLength+=strlen(currentPairs[i].string2)-strlen(currentPairs[i].string1);
                            stack+=strlen(currentPairs[i].string1)-strlen(currentPairs[i].string2);

                            matchIndex=-1;
                            startIndex=-1;
                        }
                        else if(comparisonBuffer[j]==currentPairs[i].string1[matchIndex+1] && changableIndex==-1)
                        {
                            matchIndex++;
                            if(startIndex==-1)
                                startIndex=j;

                            if(matchIndex+1==strlen(currentPairs[i].string1))
                            {
                                strcpy(buffer+startIndex-stack,currentPairs[i].string2);
                                strcpy(buffer+startIndex+strlen(currentPairs[i].string2)-stack, backupBuffer+startIndex+strlen(currentPairs[i].string1));
                                bufferLength+=strlen(currentPairs[i].string2)-strlen(currentPairs[i].string1);
                                stack+=strlen(currentPairs[i].string1)-strlen(currentPairs[i].string2);

                                matchIndex=-1;
                                startIndex=-1;
                            }
                        }
                        else if(matchIndex!=-1 && changableIndex!=-1 && strchr(currentPairs[i].changableChars[changableIndex], comparisonBuffer[j])==NULL)
                        {
                            j--;
                            matchIndex=-1;
                            startIndex=-1;
                        }
                        else if(matchIndex!=-1 && changableIndex==-1 && comparisonBuffer[j]!=currentPairs[i].string1[matchIndex+1])
                        {
                            j--;
                            matchIndex=-1;
                            startIndex=-1;
                        }
                        else if(changableIndex!=-1 && strchr(currentPairs[i].changableChars[changableIndex], comparisonBuffer[j])!=NULL)
                        {
                            matchIndex++;
                            if(startIndex==-1)
                                startIndex=j;

                            if(matchIndex+1==strlen(currentPairs[i].string1))
                            {
                                strcpy(buffer+startIndex-stack,currentPairs[i].string2);
                                strcpy(buffer+startIndex+strlen(currentPairs[i].string2)-stack, backupBuffer+startIndex+strlen(currentPairs[i].string1));
                                bufferLength+=strlen(currentPairs[i].string2)-strlen(currentPairs[i].string1);
                                stack+=strlen(currentPairs[i].string1)-strlen(currentPairs[i].string2);

                                matchIndex=-1;
                                startIndex=-1;
                            }
                        }
                    }

                }
                else if(currentPairs[i].repeatedCharCount!=0 && currentPairs[i].changableCharCount!=0)
                {
                    int j;
                    int matchIndex=-1;
                    int startIndex=-1;
                    int extraCount=0;
                    int stack=0;

                    for(j=0; j<strlen(backupBuffer); j++)
                    {
                        int changableIndex=isInIntArray(currentPairs[i].changableCharIndex, currentPairs[i].changableCharCount, matchIndex+1);
                        int repeatableIndex=isInIntArray(currentPairs[i].repeatedCharIndex, currentPairs[i].repeatedCharCount, matchIndex+1);

                        if(matchIndex+1==strlen(currentPairs[i].string1))
                        {
                            strcpy(buffer+startIndex-stack,currentPairs[i].string2);
                            strcpy(buffer+startIndex+strlen(currentPairs[i].string2)-stack, backupBuffer+startIndex+strlen(currentPairs[i].string1)+extraCount);
                            bufferLength+=strlen(currentPairs[i].string2)-strlen(currentPairs[i].string1)-extraCount;
                            stack+=strlen(currentPairs[i].string1)-strlen(currentPairs[i].string2)+extraCount;

                            matchIndex=-1;
                            startIndex=-1;
                            extraCount=0;
                        }
                        else if(comparisonBuffer[j]==currentPairs[i].string1[matchIndex+1] && repeatableIndex==-1 && changableIndex==-1)
                        {
                            matchIndex++;
                            if(startIndex==-1)
                                startIndex=j;
                        }
                        else if(comparisonBuffer[j]==currentPairs[i].string1[matchIndex+1] && repeatableIndex!=-1 && changableIndex==-1)
                        {
                            matchIndex++;
                            if(startIndex==-1)
                                startIndex=j;

                            while(j<strlen(backupBuffer)-1)
                            {
                                if(comparisonBuffer[j+1]==currentPairs[i].string1[matchIndex])
                                {
                                    j++;
                                    extraCount++;
                                }
                                else
                                    break;
                            }

                            if(matchIndex+1==strlen(currentPairs[i].string1))
                            {
                                strcpy(buffer+startIndex-stack,currentPairs[i].string2);
                                strcpy(buffer+startIndex+strlen(currentPairs[i].string2)-stack, backupBuffer+startIndex+strlen(currentPairs[i].string1)+extraCount);
                                bufferLength+=strlen(currentPairs[i].string2)-strlen(currentPairs[i].string1)-extraCount;
                                stack+=strlen(currentPairs[i].string1)-strlen(currentPairs[i].string2)+extraCount;

                                matchIndex=-1;
                                startIndex=-1;
                                extraCount=0;
                            }
                        }
                        else if(repeatableIndex==-1 && changableIndex!=-1 && strchr(currentPairs[i].changableChars[changableIndex], comparisonBuffer[j])!=NULL)
                        {
                            matchIndex++;
                            if(startIndex==-1)
                                startIndex=j;

                            if(matchIndex+1==strlen(currentPairs[i].string1))
                            {
                                strcpy(buffer+startIndex-stack,currentPairs[i].string2);
                                strcpy(buffer+startIndex+strlen(currentPairs[i].string2)-stack, backupBuffer+startIndex+strlen(currentPairs[i].string1));
                                bufferLength+=strlen(currentPairs[i].string2)-strlen(currentPairs[i].string1);
                                stack+=strlen(currentPairs[i].string1)-strlen(currentPairs[i].string2)+extraCount;

                                matchIndex=-1;
                                startIndex=-1;
                                extraCount=0;
                            }
                        }
                        else if(repeatableIndex!=-1 && changableIndex!=-1 && strchr(currentPairs[i].changableChars[changableIndex], comparisonBuffer[j])!=NULL)
                        {
                            matchIndex++;
                            if(startIndex==-1)
                                startIndex=j;
                            char* chosen=strchr(currentPairs[i].changableChars[changableIndex], comparisonBuffer[j]);

                            while(j<strlen(backupBuffer)-1)
                            {
                                if(comparisonBuffer[j+1]==*chosen)
                                {
                                    j++;
                                    extraCount++;
                                }
                                else
                                    break;
                            }

                            if(matchIndex+1==strlen(currentPairs[i].string1))
                            {
                                strcpy(buffer+startIndex-stack,currentPairs[i].string2);
                                strcpy(buffer+startIndex+strlen(currentPairs[i].string2)-stack, backupBuffer+startIndex+strlen(currentPairs[i].string1)+extraCount);
                                bufferLength+=strlen(currentPairs[i].string2)-strlen(currentPairs[i].string1)-extraCount;
                                stack+=strlen(currentPairs[i].string1)-strlen(currentPairs[i].string2)+extraCount;

                                matchIndex=-1;
                                startIndex=-1;
                                extraCount=0;
                            }
                        }
                        else if(comparisonBuffer[j]!=currentPairs[i].string1[matchIndex+1] && repeatableIndex!=-1)
                        {
                            matchIndex++;
                            j--;
                            extraCount--;

                            if(matchIndex+1==strlen(currentPairs[i].string1))
                            {
                                strcpy(buffer+startIndex-stack,currentPairs[i].string2);
                                strcpy(buffer+startIndex+strlen(currentPairs[i].string2)-stack, backupBuffer+startIndex+strlen(currentPairs[i].string1)+extraCount);
                                bufferLength+=strlen(currentPairs[i].string2)-strlen(currentPairs[i].string1)-extraCount;
                                stack+=strlen(currentPairs[i].string1)-strlen(currentPairs[i].string2)+extraCount;

                                matchIndex=-1;
                                startIndex=-1;
                                extraCount=0;
                            }
                        }
                        else if(matchIndex!=-1 && comparisonBuffer[j]!=currentPairs[i].string1[matchIndex+1] && repeatableIndex==-1)
                        {
                            if(startIndex!=-1)
                                j--;
                            matchIndex=-1;
                            startIndex=-1;
                            extraCount=0;
                        }

                        if(j==strlen(backupBuffer)-1 && isInIntArray(currentPairs[i].repeatedCharIndex, currentPairs[i].repeatedCharCount, matchIndex+1)!=-1 && matchIndex!=-1)
                        {
                            for(int k=strlen(backupBuffer)-extraCount-startIndex; k<strlen(currentPairs[i].string1); k++)
                            {
                                if(isInIntArray(currentPairs[i].repeatedCharIndex, currentPairs[i].repeatedCharCount, k)==-1)
                                    break;
                                else
                                    extraCount--;

                                if(k==strlen(currentPairs[i].string1)-1)
                                {
                                    strcpy(buffer+startIndex-stack,currentPairs[i].string2);
                                    strcpy(buffer+startIndex+strlen(currentPairs[i].string2)-stack, backupBuffer+startIndex+strlen(currentPairs[i].string1)+extraCount);
                                    bufferLength+=strlen(currentPairs[i].string2)-strlen(currentPairs[i].string1)-extraCount;

                                    break;
                                }
                            }
                        }
                    }
                }
                else if(foundSubstr!=NULL)
                {
                    backupBuffer[0]='\0';

                    strcat(backupBuffer, buffer);
                    int j=0;
                    int stack=0;

                    while(foundSubstr!=NULL && j<bufferLength)
                    {
                        for(j; j<bufferLength; j++)
                            if(comparisonBuffer+j==foundSubstr)
                            {
                                strcpy(buffer+j-stack,currentPairs[i].string2);
                                break;
                            }
                        strcpy(buffer+j+strlen(currentPairs[i].string2)-stack, backupBuffer+j+strlen(currentPairs[i].string1));
                        bufferLength+=strlen(currentPairs[i].string2)-strlen(currentPairs[i].string1);
                        stack+=strlen(currentPairs[i].string1)-strlen(currentPairs[i].string2);
                        foundSubstr=strstr(comparisonBuffer+j+1, currentPairs[i].string1);
                    }
                }
            }
        }

        strcat(buffer,following);
        int extraLength=1;
        if(bytesread==0)
            extraLength=0;

        while(((byteswritten=write(tempFile, buffer, bufferLength+extraLength))==-1) && (errno==EINTR));
        if(byteswritten<0)
        {
            perror("Cannot write to the file");
            close(inputFile);
            close(tempFile);
            unlink(tempName);
            exit(-1);
        }

        buffer[0]='\0';
        tempBuffer[0]='\0';
        backupBuffer[0]='\0';
        bufferLength=0;
        if(isLineEnd==true)
        {
            isLineStart=true;
            isLineEnd=false;
        }
        else if(isLineStart==true)
        {
            isLineStart=false;
            isLineEnd=false;
        }
        following[0]='\0';

        if(bytesread==0)
            break;
    }

    //Rewriting to the inputFile
    ftruncate(inputFile, 0);
    lseek(inputFile, 0, SEEK_SET);

    char* bp;
    char buf[10];
    byteswritten=0;

    lseek(tempFile, 0, SEEK_SET);
    while(true)
    {
        while(((bytesread=read(tempFile, buf, 10))==-1) && (errno==EINTR));
        if(bytesread<0)
        {
            perror("Cannot read from the file");
            close(inputFile);
            close(tempFile);
	    unlink(tempName);
            exit(-1);
        }
        else if(bytesread==0)
            break;
        bp=buf;
        while(bytesread>0)
        {
            while(((byteswritten=write(inputFile, bp, bytesread))==-1) && (errno==EINTR));
            if(byteswritten<0)
            {
                perror("Cannot write to the file");
                close(inputFile);
                close(tempFile);
	        unlink(tempName);
                exit(-1);
            }
            bytesread-=byteswritten;
            bp+=byteswritten;
        }
        if(byteswritten==-1)
            break;
    }

    //Unlocking the file
    lock.l_type=F_UNLCK;
    fcntl(inputFile, F_SETLKW, &lock);
    while(((fcntl(inputFile, F_SETLKW, &lock))==-1) && errno==EINTR);

    printf("The editing is completed.\n");
    close(inputFile);
    close(tempFile);
    unlink(tempName);

    return 0;
}

int isInIntArray(int* array, int arraySize, int element)
{
    for(int i=0; i<arraySize; ++i)
        if(array[i]==element)
            return i;

    return -1;
}
