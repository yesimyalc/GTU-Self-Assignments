#include "clientServerCommunication.h"

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
    //Locking the file for writing
    struct flock lock;
    memset(&lock, 0, sizeof(lock));
    lock.l_type=F_WRLCK;

    while(((fcntl(fileFd, F_SETLKW, &lock))==-1) && errno==EINTR);
    while(((byteswritten=write(fileFd, output, strlen(output)))==-1) && (errno==EINTR));

    //Unlocking the file
    lock.l_type=F_UNLCK;
    while(((fcntl(fileFd, F_SETLKW, &lock))==-1) && errno==EINTR);

    return byteswritten;
}

char* addCharacterToLine(char newCharacter, char** oldLine, int* oldLineSize)
{
    if(*oldLineSize==0)
    {
        *oldLine=(char*)malloc(2);
        if(oldLine==NULL)
            return NULL;
        (*oldLine)[0]=newCharacter;
        (*oldLine)[1]='\0';
        (*oldLineSize)++;
        return *oldLine;
    }

    char* backupNumber=(char*)malloc(*oldLineSize);
    if(backupNumber==NULL)
        return NULL;

    for(int i=0; i<*oldLineSize; ++i)
        backupNumber[i]=(*oldLine)[i];

    *oldLine=(char*)realloc(*oldLine, (*oldLineSize)+2);
    if(*oldLine==NULL)
    {
        free(backupNumber);
        return(NULL);
    }
    for(int i=0; i<*oldLineSize; ++i)
        (*oldLine)[i]=backupNumber[i];
    (*oldLine)[*oldLineSize]=newCharacter;
    (*oldLine)[*oldLineSize+1]='\0';

    (*oldLineSize)++;

    free(backupNumber);

    return *oldLine;
}

void freePointer(void* pointer)
{
    if(pointer!=NULL)
        free(pointer);
}

struct cityInfo* addCity(struct cityInfo currentCity, struct cityInfo** allCities, int *cityCount)
{
    if(*cityCount==0)
    {
        *allCities=(struct cityInfo*)malloc(sizeof(struct cityInfo));
        if(allCities==NULL)
            return NULL;
        (*allCities)[0]=currentCity;
        (*cityCount)++;
        return *allCities;
    }

    struct cityInfo* backupArray=(struct cityInfo*)malloc((*cityCount)*sizeof(struct cityInfo));
    if(backupArray==NULL)
        return NULL;

    for(int i=0; i<*cityCount; ++i)
        backupArray[i]=(*allCities)[i];

    *allCities=(struct cityInfo*)realloc(*allCities, (*cityCount)*sizeof(struct cityInfo)+sizeof(struct cityInfo));
    if(*allCities==NULL)
    {
        free(backupArray);
        return(NULL);
    }
    for(int i=0; i<*cityCount; ++i)
        (*allCities)[i]=backupArray[i];
    (*allCities)[*cityCount]=currentCity;

    (*cityCount)++;
    free(backupArray);

    return *allCities;
}


void retrieveRequestData(char* requestStringG, struct request* newRequest)
{
    char* word;
    int counter=0;

    char requestString[strlen(requestStringG)];
    strcpy(requestString, requestStringG);
    char* buffer=requestString;

    while((word=strtok_r(buffer," ",&buffer)) !=NULL)
    {
        if(counter==1)
            strcpy(newRequest->type, word);
        else if(counter==2)
            strcpy(newRequest->startDate, word);
        else if(counter==3)
            strcpy(newRequest->endDate, word);
        else if(counter==4)
            strcpy(newRequest->city, word);
        else if(counter!=0)
            break;

        counter++;
    }
}

char* arrangeClientSocketInput(char** socketInput, struct request currentRequest)
{
    int allocationSize=2+3+strlen(currentRequest.type)+strlen(currentRequest.startDate)+strlen(currentRequest.endDate)+strlen(currentRequest.city)+2;

    *socketInput=(char*)malloc(allocationSize);
    if(*socketInput==NULL)
        return NULL;

    strcpy(*socketInput, "c ");
    strcat(*socketInput, currentRequest.type);
    strcat(*socketInput, " ");
    strcat(*socketInput, currentRequest.startDate);
    strcat(*socketInput, " ");
    strcat(*socketInput, currentRequest.endDate);
    strcat(*socketInput, " ");
    strcat(*socketInput, currentRequest.city);
    strcat(*socketInput, "\n");

    (*socketInput)[allocationSize-1]='\0';

    return (*socketInput);
}
