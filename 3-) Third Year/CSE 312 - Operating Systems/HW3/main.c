#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include "filesystem.h"

void checkArgc(int count);

int main(int argc, char *argv[])
{
    setenv("TZ", "Europe/Istanbul", 1);
    tzset();

    //Checking the command line arguments
    checkArgc(argc);

    //Retrieving the command line inputs
    int blockSize=atoi(argv[1])*KB_1;
    int iNodeCount=atoi(argv[2]);
    char* filePath=argv[3];
    if(blockSize<KB_1 || blockSize>16*MB_1)
    {
        printf("The blocksize cannot be smaller than 1KB or bigger than 16MB.\n");
        exit(EXIT_FAILURE);
    }
    else if(iNodeCount<2 || iNodeCount*sizeof(struct iNode)>16*MB_1)
    {
        printf("The iNodeCount cannot be smaller than 2 and the iNode space cannot be bigger than 16MB.\n");
        exit(EXIT_FAILURE);
    }

    //Calculating the sizes of iNode space and block space
    int iNodeSpaceSize=sizeof(struct iNode)*iNodeCount;
    int blockSpaceSize=16*MB_1-sizeof(struct superblock)-iNodeSpaceSize;
    if(blockSpaceSize<blockSize)
    {
        printf("There cannot be any blocks with the given iNode count and block size. Change their numbers and try again.\n");
        exit(EXIT_FAILURE);
    }

    int blockCount=blockSpaceSize/blockSize;

    //Creating the superblock
    struct superblock* s=(struct superblock*)fileSystem;
    s->blockSize=blockSize;
    s->blockCount=blockCount;
    s->firstBlockAdd=16*MB_1-blockSpaceSize;
    s->iNodeCount=iNodeCount;
    s->firstINodeAdd=s->firstBlockAdd-iNodeSpaceSize;
    for(int i=0; i<blockCount; ++i)
    {
        if(i==0)
            s->freeBlocks[i]=DIR;
        else
            s->freeBlocks[i]=FREE;
    }

    //Creating the root
    struct iNode* r=(struct iNode*)(fileSystem+s->firstINodeAdd);
    strcpy(r->name, "/");
    r->size=sizeof(file)*2;
    r->creationDate=time(NULL);
    r->modificationDate=time(NULL);
    r->isFree=DIR;
    for(int i=0; i<7; ++i)
    {
        if(i==0)
            r->directAdd[i]=0;
        else
            r->directAdd[i]=-1;
    }
    r->singleAdd[0]=-1;
    r->doubleAdd[0]=-1;
    r->tripleAdd[0]=-1;
    struct file* f=(struct file*)(fileSystem+s->firstBlockAdd);
    f->iNodeNo=0;
    strcpy(f->fileName, ".");
    f++;
    f->iNodeNo=0;
    strcpy(f->fileName, "..");

    //Initializing rest of the iNodes
    for(int i=1; i<iNodeCount; ++i)
    {
        struct iNode* r=(struct iNode*)(fileSystem+s->firstINodeAdd+sizeof(struct iNode)*i);
        strcpy(r->name, "NULL");
        r->size=0;
        r->creationDate=-1;
        r->modificationDate=-1;
        r->isFree=FREE;
        for(int i=0; i<7; ++i)
            r->directAdd[i]=-1;
        r->singleAdd[0]=-1;
        r->doubleAdd[0]=-1;
        r->tripleAdd[0]=-1;
    }

    //Writing to the filesystem file
    FILE *file;
    file=fopen(filePath, "wb+");
    if(NULL==file)
    {
        perror("Failed to open the system file");
        exit(EXIT_FAILURE);
    }

    if(1!=fwrite(fileSystem, (16*MB_1), 1, file))
    {
        perror("Failed to write to the file");
        fclose(file);
        return EXIT_FAILURE;
    }
    fclose(file);

    return 0;
}

void checkArgc(int count)
{
    if(count!=4)
    {
        fprintf(stderr, "You need exactly 4 command line arguments (first being auto inputed) to run the program.\n");
        fprintf(stderr, "1-)BlockSize, 2-)INodeCount, 3-)FileSystem file. Please try again.\n");
        exit(EXIT_FAILURE);
    }
}

