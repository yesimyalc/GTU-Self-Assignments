#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "filesystem.h"

void retrieveArguments(int argc, char* arguements[], char** systemP, char** opName, char** opP, char** fileP);
void printSingleLinkBlocks(struct superblock* s, uint16_t blockNo);
void printFileSystemInfo(struct superblock* s, struct iNode* iN);
void retrieveParents(int iNodeCount, char array[iNodeCount][1000], int* count, char* path);
int isIn2DCharArray(int size1, int size2, char array[size1][size2], int loopSize, char* element);
int checkPathValidity(int size1, int size2, char array[size1][size2], int count, struct superblock* s, struct iNode* iN);
int findParentINode(int size1, int size2, char array[size1][size2], int count, int numberInArray, struct superblock* s, struct iNode* iN);
int findFreeBlock(struct superblock* s);
int getAvailableBlock(struct iNode* iN, struct superblock* s);
void createDirectory(char* directoryPath, struct superblock* s, struct iNode* iN);
void removeDirFromDir(struct iNode* parent, int deletingInode, struct superblock* s);
struct file* findReplacementFile(struct iNode* dirINode, struct superblock* s);
void removeDirectory(char* directoryPath, struct superblock* s, struct iNode* iN);
void printDirectoryInfo(struct superblock* s, struct iNode* iN, char* directoryPath);
void writeToFile(char* path, char* fileName, struct superblock* s, struct iNode* iN);
void readFromFile(char* path, char* fileName, struct superblock* s, struct iNode* iN);
void removeFile(char* path, struct superblock* s, struct iNode* iN);

int main(int argc, char *argv[])
{
    setenv("TZ", "Europe/Istanbul", 1);
    tzset();

    //Retrieving command line arguments and understanding the operation
    char* systemPath=NULL;
    char* operation=NULL;
    char* operationPath=NULL;
    char* filePath=NULL;
    retrieveArguments(argc, argv, &systemPath, &operation, &operationPath, &filePath);

    //Opening the file system file and saving it to memory
    FILE *file;
    file=fopen(systemPath, "rb");
    if(NULL==file)
    {
        perror("Cannot open the system file");
        exit(EXIT_FAILURE);
    }
    if(1!=fread(fileSystem, 16*MB_1, 1, file))
    {
        perror("Cannot read the filesystem");
        exit(EXIT_FAILURE);
    }
    fclose(file);

    struct superblock* sBlock=(struct superblock*)fileSystem;
    struct iNode* iNodes=(struct iNode*)(fileSystem+sBlock->firstINodeAdd);

    //Doing the operation and making changes in the filesystem
    if(strcmp(operation, "dir")==0)
        printDirectoryInfo(sBlock, iNodes, operationPath);
    else if(strcmp(operation, "mkdir")==0)
    {
        if(strcmp(operationPath, "/")==0)
        {
            fprintf(stderr, "You cannot create another root directory.\n");
            exit(EXIT_FAILURE);
        }
        createDirectory(operationPath, sBlock, iNodes);
    }
    else if(strcmp(operation, "rmdir")==0)
    {
        if(strcmp(operationPath, "/")==0)
        {
            fprintf(stderr, "You cannot delete root directory.\n");
            exit(EXIT_FAILURE);
        }
        removeDirectory(operationPath, sBlock, iNodes);
    }
    else if(strcmp(operation, "dumpe2fs")==0)
        printFileSystemInfo(sBlock, iNodes);
    else if(strcmp(operation, "write")==0)
    {
        if(strcmp(operationPath, "/")==0)
        {
            fprintf(stderr, "\"/\" is not a valid file name.\n");
            exit(EXIT_FAILURE);
        }
        writeToFile(operationPath, filePath, sBlock, iNodes);
    }
    else if(strcmp(operation, "read")==0)
    {
        if(strcmp(operationPath, "/")==0)
        {
            fprintf(stderr, "\"/\" is not a valid file name.\n");
            exit(EXIT_FAILURE);
        }
        readFromFile(operationPath, filePath, sBlock, iNodes);
    }
    else if(strcmp(operation, "del")==0)
    {
        if(strcmp(operationPath, "/")==0)
        {
            fprintf(stderr, "\"/\" is not a valid file name.\n");
            exit(EXIT_FAILURE);
        }
        removeFile(operationPath, sBlock, iNodes);
    }

    //Saving the changes to the filesystem
    file=fopen(systemPath, "wb+");
    if(NULL==file)
    {
        perror("Failed to open the system file to save changes");
        exit(EXIT_FAILURE);
    }
    if(1!=fwrite(fileSystem, 16*MB_1, 1, file))
    {
        perror("Failed to write the changes into the file system");
        exit(EXIT_FAILURE);
    }
    fclose(file);

    return 0;
}

void retrieveArguments(int argc, char* arguements[], char** systemP, char** opName, char** opP, char** fileP)
{
    if(argc<3 || argc>5)
    {
        fprintf(stderr, "There cannot be less than 3 arguments or more than 5 arguments.\n");
        exit(EXIT_FAILURE);
    }

    *systemP=arguements[1];
    *opName=arguements[2];
    if(argc==3)
    {
        if(*systemP==NULL || *opName==NULL || strcmp(*opName, "dumpe2fs")!=0)
        {
            fprintf(stderr, "The command line arguments are invalid. You have entered 2 arguments. First one should be the system file. Second one should be dumpe2fs.\n");
            exit(EXIT_FAILURE);
        }
    }
    else if(argc==4)
    {
        *opP=arguements[3];
        if(*systemP==NULL || (*opName==NULL && strcmp(*opName, "dir")!=0 && strcmp(*opName, "mkdir")!=0 && strcmp(*opName, "rmdir")!=0 && strcmp(*opName, "del")!=0) || *opP==NULL)
        {
            fprintf(stderr, "The command line arguments are invalid. You have entered 3 arguments. First one should be the system file.\n");
            fprintf(stderr, "Second one should be dir, mkdir, rmdir or del.\n");
            fprintf(stderr, "Third one should be the path in the file system.\n");
            exit(EXIT_FAILURE);
        }
    }
    else if(argc==5)
    {
        *opP=arguements[3];
        *fileP=arguements[4];
        if(*systemP==NULL || (*opName==NULL && strcmp(*opName, "write")!=0 && strcmp(*opName, "read")!=0) || *opP==NULL || fileP==NULL)
        {
            fprintf(stderr, "The command line arguments are invalid. You have entered 4 arguments. First one should be the system file.\n");
            fprintf(stderr, "Second one should be dir, mkdir, rmdir or del.\n");
            fprintf(stderr, "Third one should be the path in the file system and fourth one should be a file name.\n");
            exit(EXIT_FAILURE);
        }
    }

    if(argc>3 && ((arguements[3][0]!='/' && arguements[3][0]!='.') || (arguements[3][strlen(arguements[3])-1]=='/' && strlen(arguements[3])!=1)))
    {
        fprintf(stderr, "The operation path is not provided properly. The path should start with / or .\n");
        exit(EXIT_FAILURE);
    }
}

void printSingleLinkBlocks(struct superblock* s, uint16_t blockNo)
{
    struct addressBlock* block=(struct addressBlock*)(fileSystem+(s->firstBlockAdd)+(blockNo)*s->blockSize);
    for(int k=0; k<100; k++)
        if(block->directAdd[k]!=-1)
            printf("%d ", block->directAdd[k]);
}

void printFileSystemInfo(struct superblock* s, struct iNode* iN)
{
    int freeBlocks=0;
    int freeInodes=0;
    int directories=0;
    int files=0;

    struct iNode* iteratorINode=iN;

    for(int i=0; i<s->iNodeCount; i++)
    {
        if(iteratorINode->isFree==FREE)
            freeInodes++;
        else if(iteratorINode->isFree==DIR)
            directories++;
        else if(iteratorINode->isFree==FIL)
            files++;
        iteratorINode++;
    }

    for(int i=0; i<s->blockCount; ++i)
    {
        if(s->freeBlocks[i]==FREE)
            freeBlocks++;
    }

    printf("*****Information of the File System*****\n");
    printf("Block Related Information\n");
    printf("Total Block Count: %d\n", s->blockCount);
    printf("Block Size: %dKB\n", (s->blockSize)/KB_1);
    printf("Total Block Space: %dMB\n\n", (s->blockCount*s->blockSize)/MB_1);

    printf("Other Space Related Information\n");
    printf("Total Superblock Space: %dKB\n", (int)sizeof(struct superblock)/KB_1);
    printf("Total INode Count: %d\n", s->iNodeCount);
    printf("INode Size: %dB\n", (int)sizeof(struct iNode));
    printf("Total INode Space: %dKB\n", (int)(s->iNodeCount*sizeof(struct iNode))/KB_1);
    printf("Total Size: 16MB\n\n");

    printf("File Related Information\n");
    printf("Total Free Block Count: %d\n", freeBlocks);
    printf("Total Free Inode Count: %d\n", freeInodes);
    printf("Total Directory Count: %d\n", directories);
    printf("Total File Count: %d\n\n", files);

    for(int i=0; i<s->iNodeCount; ++i)
    {
        struct file* f=(struct file*)(fileSystem+(s->firstBlockAdd)+(iN->directAdd[0]*s->blockSize));
        if(iN->isFree==DIR)
            printf("Directory: \"%s\", INode: %d\nParent Directory: \"%s\", Parent Directory INode: %d, Size: %dB\nOccupying blocks: ", iN->name, i, f->fileName, f->iNodeNo, iN->size);

        else if(iN->isFree==FIL)
            printf("File: \"%s\", INode: %d\nParent Directory: \"%s\", Parent Directory INode: %d, Size: %dB\nOccupying blocks: ", iN->name, i, f->fileName, f->iNodeNo, iN->size);

        for(int j=0; j<7; ++j)
            if(iN->directAdd[j]!=-1)
                printf("%d ", iN->directAdd[j]);

        if(iN->singleAdd[0]!=-1)
            printSingleLinkBlocks(s, iN->singleAdd[0]);
        if(iN->doubleAdd[0]!=-1)
        {
            struct singleLink* sBlock=(struct singleLink*)(fileSystem+(s->firstBlockAdd)+(s->blockSize)*(iN->doubleAdd[0]));
            for(int j=0; j<100; j++)
                if(sBlock->singleAdd[j]!=-1)
                    printSingleLinkBlocks(s, sBlock->singleAdd[j]);
        }
        if(iN->tripleAdd[0]!=-1)
        {
            struct doubleLink* dBlock=(struct doubleLink*)(fileSystem+(s->firstBlockAdd)+(s->blockSize)*(iN->tripleAdd[0]));
            for(int j=0; j<100; j++)
                if(dBlock->doubleAdd[j]!=-1)
                {
                    struct singleLink* sBlock=(struct singleLink*)(fileSystem+(s->firstBlockAdd)+(s->blockSize)*(dBlock->doubleAdd[j]));
                    for(int k=0; k<100; k++)
                        if(sBlock->singleAdd[j]!=-1)
                            printSingleLinkBlocks(s, sBlock->singleAdd[j]);
                }
        }
        if(iN->isFree!=FREE)
        {
            if((iN->size)%(s->blockSize)!=0)
                printf("\nLast printed block is fragmented.");
            printf("\n\n");
        }

        if((i+1)==directories+files)
            break;

        iN++;
    }
}

void retrieveParents(int iNodeCount, char array[iNodeCount][1000], int* count, char* path)
{
    if(strlen(path)==1)
    {
        strcpy(array[0], "/");
        *count=1;
        return;
    }

    int startingIndex=0;
    if(strlen(path)>1 && path[0]=='.' && path[1]=='.')
        startingIndex=1;
    else
        startingIndex=0;

    strcpy(array[0], "/");
    (*count)++;

    char* string=path+startingIndex;
    while(true)
    {
        char* endString=strchr(string+1, '/');
        if(endString!=NULL)
            strncpy(array[(*count)++], string+1, strlen(string+1)-strlen(endString));
        else
        {
            strncpy(array[(*count)++], string+1, strlen(string+1));
            break;
        }
        string=endString;
    }

    if(strlen(array[(*count)-1])>13)
    {
        fprintf(stderr, "Given filename is too long\n");
        exit(EXIT_FAILURE);
    }
}

int isIn2DCharArray(int size1, int size2, char array[size1][size2], int loopSize, char* element)
{
    for(int i=0; i<loopSize; ++i)
    {
        if(strcmp(array[i], element)==0)
            return i;
    }

    return -1;
}

int checkPathValidity(int size1, int size2, char array[size1][size2], int count, struct superblock* s, struct iNode* iN)
{
    if(count==1)
        return 0;

    int validCount1=1;
    int validCount2=1;
    struct iNode* inode=iN;
    struct iNode* iteratorIN=iN;
    for(int i=0; i<s->iNodeCount; ++i)
    {
        int index=isIn2DCharArray(size1, size2, array, count, inode->name);
        if(index==count-1)
        {
            int j=0;
            for(j=0; j<count-1; j++)
            {
                struct file* f=(struct file*)(fileSystem+(s->firstBlockAdd)+(inode->directAdd[0]*s->blockSize));
                inode=iN+(f->iNodeNo);
                if(strcmp(inode->name, array[count-validCount1-1])!=0)
                    break;
                else
                    validCount1++;
            }
            if(j!=count-1)
                validCount1=1;
        }
        else if(index==count-2 && validCount2!=count-1)
        {
            for(int j=0; j<count-2; j++)
            {
                struct file* f=(struct file*)(fileSystem+(s->firstBlockAdd)+(inode->directAdd[0]*s->blockSize));
                inode=iN+(f->iNodeNo);
                if(strcmp(inode->name, array[count-validCount2-2])!=0)
                {
                    validCount2=1;
                    break;
                }
                else
                    validCount2++;
            }
        }

        iteratorIN++;
        inode=iteratorIN;

        if(validCount1==count)
        {
            validCount1=i;
            break;
        }
        else if(validCount1!=count && i==(s->iNodeCount)-1)
            validCount1=-1;
    }

    if(validCount1>=0)
        return validCount1;
    else if(validCount2==count-1)
        return -2;
    else
        return -1;
}

int findParentINode(int size1, int size2, char array[size1][size2], int count, int numberInArray, struct superblock* s, struct iNode* iN)
{
    if(count==1)
        return 0;

    if(numberInArray==0)
        return 0;

    int validCount1=1;
    struct iNode* inode=iN;
    struct iNode* iteratorIN=iN;
    for(int i=0; i<s->iNodeCount; ++i)
    {
        int index=isIn2DCharArray(size1, size2, array, count, inode->name);
        if(index==numberInArray-1)
        {
            int j=0;
            for(j=0; j<numberInArray-1; j++)
            {
                struct file* f=(struct file*)(fileSystem+(s->firstBlockAdd)+(inode->directAdd[0]*s->blockSize));
                inode=iN+(f->iNodeNo);
                if(strcmp(inode->name, array[numberInArray-validCount1-1])!=0)
                    break;
                else
                    validCount1++;
            }
            if(j!=numberInArray-1)
                validCount1=1;
        }

        iteratorIN++;
        inode=iteratorIN;

        if(validCount1==numberInArray)
        {
            validCount1=i;
            break;
        }
    }

    return validCount1;
}

int findFreeBlock(struct superblock* s)
{
    for(int i=0; i<s->blockCount; ++i)
        if(s->freeBlocks[i]==FREE)
            return i;
    return -1;
}

int getAvailableBlock(struct iNode* iN, struct superblock* s)
{
    int index=(iN->size)/(s->blockSize);

    if(index<7)
    {
        if(iN->directAdd[index]==-1)
        {
            int newBlock=findFreeBlock(s);
            if(newBlock==-1)
            {
                fprintf(stderr, "There is no free blocks\n");
                exit(EXIT_FAILURE);
            }
            iN->directAdd[index]=newBlock;
            s->freeBlocks[newBlock]=FIL;
        }
        return ((iN->directAdd[index])*s->blockSize)+((iN->size)-((s->blockSize)*index));
    }
    else if(index>=7 && index<107)
    {
        index-=7;
        if(iN->singleAdd[0]==-1)
        {
            int newBlock=findFreeBlock(s);
            if(newBlock==-1)
            {
                fprintf(stderr, "There is no free blocks\n");
                exit(EXIT_FAILURE);
            }
            iN->singleAdd[0]=newBlock;
            s->freeBlocks[newBlock]=FIL;
            struct addressBlock* single=(struct addressBlock*)(fileSystem+(s->firstBlockAdd)+(iN->singleAdd[0])*s->blockSize);
            for(int k=0; k<100; k++)
                single->directAdd[k]=-1;
        }
        struct addressBlock* single=(struct addressBlock*)(fileSystem+(s->firstBlockAdd)+(iN->singleAdd[0])*s->blockSize);

        if(single->directAdd[index]==-1)
        {
            int newBlock=findFreeBlock(s);
            if(newBlock==-1)
            {
                fprintf(stderr, "There is no free blocks\n");
                exit(EXIT_FAILURE);
            }
            single->directAdd[index]=newBlock;
            s->freeBlocks[newBlock]=FIL;
        }
        return ((single->directAdd[index])*s->blockSize)+((iN->size)-((s->blockSize)*(index+7)));
    }
    else if(index>=107 && index<10107)
    {
        index-=107;
        int firstIndex=0;
        while(index>=100)
        {
            index-=100;
            firstIndex++;
        }

        if(iN->doubleAdd[0]==-1)
        {
            int newBlock=findFreeBlock(s);
            if(newBlock==-1)
            {
                fprintf(stderr, "There is no free blocks\n");
                exit(EXIT_FAILURE);
            }
            iN->doubleAdd[0]=newBlock;
            s->freeBlocks[newBlock]=FIL;
            struct singleLink* doubleL=(struct singleLink*)(fileSystem+(s->firstBlockAdd)+(iN->doubleAdd[0])*s->blockSize);
            for(int k=0; k<100; k++)
                doubleL->singleAdd[k]=-1;
        }
        struct singleLink* doubleL=(struct singleLink*)(fileSystem+(s->firstBlockAdd)+(iN->doubleAdd[0])*s->blockSize);

        if(doubleL->singleAdd[firstIndex]==-1)
        {
            int newBlock=findFreeBlock(s);
            if(newBlock==-1)
            {
                fprintf(stderr, "There is no free blocks\n");
                exit(EXIT_FAILURE);
            }
            doubleL->singleAdd[firstIndex]=newBlock;
            s->freeBlocks[newBlock]=FIL;
            struct addressBlock* single=(struct addressBlock*)(fileSystem+(s->firstBlockAdd)+(doubleL->singleAdd[firstIndex])*s->blockSize);
            for(int k=0; k<100; k++)
                single->directAdd[k]=-1;
        }
        struct addressBlock* single=(struct addressBlock*)(fileSystem+(s->firstBlockAdd)+(doubleL->singleAdd[firstIndex])*s->blockSize);

        if(single->directAdd[index]==-1)
        {
            int newBlock=findFreeBlock(s);
            if(newBlock==-1)
            {
                fprintf(stderr, "There is no free blocks\n");
                exit(EXIT_FAILURE);
            }
            single->directAdd[index]=newBlock;
            s->freeBlocks[newBlock]=FIL;
        }
        return ((single->directAdd[index])*s->blockSize)+((iN->size)-((s->blockSize)*(index+107+(100*firstIndex))));
    }
    else
    {
        index-=10107;
        int firstIndex=0;
        int secondIndex=0;
        while(index>=10000)
        {
            index-=10000;
            firstIndex++;
        }
        while(index>=100)
        {
            index-=100;
            secondIndex++;
        }

        if(iN->tripleAdd[0]==-1)
        {
            int newBlock=findFreeBlock(s);
            if(newBlock==-1)
            {
                fprintf(stderr, "There is no free blocks\n");
                exit(EXIT_FAILURE);
            }
            iN->tripleAdd[0]=newBlock;
            s->freeBlocks[newBlock]=FIL;
            struct doubleLink* triple=(struct doubleLink*)(fileSystem+(s->firstBlockAdd)+(iN->tripleAdd[0])*s->blockSize);
            for(int k=0; k<100; k++)
                triple->doubleAdd[k]=-1;
        }
        struct doubleLink* triple=(struct doubleLink*)(fileSystem+(s->firstBlockAdd)+(iN->tripleAdd[0])*s->blockSize);

        if(triple->doubleAdd[firstIndex]==-1)
        {
            int newBlock=findFreeBlock(s);
            if(newBlock==-1)
            {
                fprintf(stderr, "There is no free blocks\n");
                exit(EXIT_FAILURE);
            }
            triple->doubleAdd[firstIndex]=newBlock;
            s->freeBlocks[newBlock]=FIL;
            struct singleLink* doubleL=(struct singleLink*)(fileSystem+(s->firstBlockAdd)+(triple->doubleAdd[firstIndex])*s->blockSize);
            for(int k=0; k<100; k++)
                doubleL->singleAdd[k]=-1;
        }
        struct singleLink* doubleL=(struct singleLink*)(fileSystem+(s->firstBlockAdd)+(triple->doubleAdd[firstIndex])*s->blockSize);

        if(doubleL->singleAdd[secondIndex]==-1)
        {
            int newBlock=findFreeBlock(s);
            if(newBlock==-1)
            {
                fprintf(stderr, "There is no free blocks\n");
                exit(EXIT_FAILURE);
            }
            doubleL->singleAdd[secondIndex]=newBlock;
            s->freeBlocks[newBlock]=FIL;
            struct addressBlock* single=(struct addressBlock*)(fileSystem+(s->firstBlockAdd)+(doubleL->singleAdd[secondIndex])*s->blockSize);
            for(int k=0; k<100; k++)
                single->directAdd[k]=-1;
        }
        struct addressBlock* single=(struct addressBlock*)(fileSystem+(s->firstBlockAdd)+(doubleL->singleAdd[secondIndex])*s->blockSize);

         if(single->directAdd[index]==-1)
        {
            int newBlock=findFreeBlock(s);
            if(newBlock==-1)
            {
                fprintf(stderr, "There is no free blocks\n");
                exit(EXIT_FAILURE);
            }
            single->directAdd[index]=newBlock;
            s->freeBlocks[newBlock]=FIL;
        }
        return ((single->directAdd[index])*s->blockSize)+((iN->size)-((s->blockSize)*(index+10107+(firstIndex*10000)+(secondIndex*100))));
    }

}

void createDirectory(char* directoryPath, struct superblock* s, struct iNode* iN)
{
    //Finding the parents of the new directory
    char parents[s->iNodeCount][1000];
    int directoryCountInPath=0;
    int iNodeNumber=0;
    retrieveParents(s->iNodeCount, parents, &directoryCountInPath, directoryPath);

    //Checking validity and finding a free iNode
    struct iNode* newInode=NULL;
    int status=checkPathValidity(s->iNodeCount, 1000, parents, directoryCountInPath, s, iN);
    if(status==-1)
    {
        fprintf(stderr, "The given path does not exist.\n");
        exit(EXIT_FAILURE);
    }
    else if(status>=0)
    {
        fprintf(stderr, "Given directory already exists.\n");
        exit(EXIT_FAILURE);
    }

    for(int i=0; i<s->iNodeCount; ++i)
    {
        if(newInode==NULL && iN->isFree==FREE)
        {
            newInode=iN;
            iNodeNumber=i;
            break;
        }
        else
            iN++;
    }

    if(newInode==NULL)
    {
        fprintf(stderr, "No iNode is available for a new directory.\n");
        exit(EXIT_FAILURE);
    }

    //Finding a free block
    int freeBlockNo=findFreeBlock(s);
    if(freeBlockNo==-1)
    {
        fprintf(stderr, "No blocks are available.\n");
        exit(EXIT_FAILURE);
    }

    //Creating the directory
    strcpy(newInode->name, parents[directoryCountInPath-1]);
    newInode->size=sizeof(file)*2;
    newInode->creationDate=time(NULL);
    newInode->modificationDate=time(NULL);
    newInode->isFree=DIR;
    for(int i=0; i<7; ++i)
    {
        if(i==0)
            newInode->directAdd[i]=freeBlockNo;
        else
            newInode->directAdd[i]=-1;
    }
    newInode->singleAdd[0]=-1;
    newInode->doubleAdd[0]=-1;
    newInode->tripleAdd[0]=-1;
    struct file* f=(struct file*)(fileSystem+(s->firstBlockAdd)+(s->blockSize)*freeBlockNo);
    f->iNodeNo=findParentINode(s->iNodeCount, 1000, parents, directoryCountInPath, directoryCountInPath-1, s, (struct iNode*)(fileSystem+s->firstINodeAdd));
    strcpy(f->fileName, parents[directoryCountInPath-2]);
    f++;
    f->iNodeNo=findParentINode(s->iNodeCount, 1000, parents, directoryCountInPath, directoryCountInPath-2, s, (struct iNode*)(fileSystem+s->firstINodeAdd));
    if(directoryCountInPath-3!=-1)
        strcpy(f->fileName, parents[directoryCountInPath-3]);
    else
        strcpy(f->fileName, "..");

    s->freeBlocks[freeBlockNo]=DIR;

    //Putting the directory as a file to it's parent
    iN=(struct iNode*)(fileSystem+s->firstINodeAdd);
    struct iNode* parentDirectory=iN+findParentINode(s->iNodeCount, 1000, parents, directoryCountInPath, directoryCountInPath-1, s, (struct iNode*)(fileSystem+s->firstINodeAdd));
    int blockAddress=getAvailableBlock(parentDirectory, s);
    parentDirectory->size+=sizeof(struct file);
    parentDirectory->modificationDate=time(NULL);
    f=(struct file*)(fileSystem+(s->firstBlockAdd)+blockAddress);
    f->iNodeNo=iNodeNumber;
    strcpy(f->fileName, parents[directoryCountInPath-1]);
}

struct file* findReplacementFile(struct iNode* dirINode, struct superblock* s)
{
    int totalFiles=(dirINode->size)/sizeof(struct file);
    int maxFileInBlock=s->blockSize/sizeof(struct file);
    int lastBlockNo=(totalFiles/maxFileInBlock);
    int filesInLastBlock=totalFiles-maxFileInBlock*(lastBlockNo)-1;
    struct file* replacement;

    if(lastBlockNo<7)
        replacement=(struct file*)(fileSystem+(s->firstBlockAdd)+(s->blockSize*dirINode->directAdd[lastBlockNo])+sizeof(struct file)*filesInLastBlock);
    else if(lastBlockNo>=7 && lastBlockNo<107)
    {
        lastBlockNo-=7;
        struct addressBlock* single=(struct addressBlock*)(fileSystem+(s->firstBlockAdd)+(dirINode->singleAdd[0])*s->blockSize);
        replacement=(struct file*)(fileSystem+(s->firstBlockAdd)+(s->blockSize*single->directAdd[lastBlockNo])+sizeof(struct file)*filesInLastBlock);
    }
    else if(lastBlockNo>=107 && lastBlockNo<10107)
    {
        lastBlockNo-=107;
        int firstIndex=0;
        while(lastBlockNo>=100)
        {
            lastBlockNo-=100;
            firstIndex++;
        }
        struct singleLink* doubleL=(struct singleLink*)(fileSystem+(s->firstBlockAdd)+(dirINode->doubleAdd[0])*s->blockSize);
        struct addressBlock* single=(struct addressBlock*)(fileSystem+(s->firstBlockAdd)+(doubleL->singleAdd[firstIndex])*s->blockSize);
        replacement=(struct file*)(fileSystem+(s->firstBlockAdd)+(s->blockSize*single->directAdd[lastBlockNo])+sizeof(struct file)*filesInLastBlock);
    }
    else
    {
        lastBlockNo-=10107;
        int firstIndex=0;
        int secondIndex=0;
        while(lastBlockNo>=10000)
        {
            lastBlockNo-=10000;
            firstIndex++;
        }
        while(lastBlockNo>=100)
        {
            lastBlockNo-=100;
            secondIndex++;
        }
        struct doubleLink* triple=(struct doubleLink*)(fileSystem+(s->firstBlockAdd)+(dirINode->tripleAdd[0])*s->blockSize);
        struct singleLink* doubleL=(struct singleLink*)(fileSystem+(s->firstBlockAdd)+(triple->doubleAdd[firstIndex])*s->blockSize);
        struct addressBlock* single=(struct addressBlock*)(fileSystem+(s->firstBlockAdd)+(doubleL->singleAdd[secondIndex])*s->blockSize);
        replacement=(struct file*)(fileSystem+(s->firstBlockAdd)+(s->blockSize*single->directAdd[lastBlockNo])+sizeof(struct file)*filesInLastBlock);
    }

    return replacement;
}

void removeDirFromDir(struct iNode* parent, int deletingInode, struct superblock* s)
{
    int maxFileInBlock=s->blockSize/sizeof(struct file);
    int fileInBlock=0;
    int end=0;

    for(int i=0; i<7; i++)
    {
        struct file* f=(struct file*)(fileSystem+(s->firstBlockAdd)+(s->blockSize)*(parent->directAdd[i]));
        while(fileInBlock!=maxFileInBlock)
        {
            if(f->iNodeNo==deletingInode)
            {
                struct file* lastFile=findReplacementFile(parent, s);
                strcpy(f->fileName, lastFile->fileName);
                f->iNodeNo=lastFile->iNodeNo;
                end=1;
                break;
            }
            f++;
            fileInBlock++;
        }
        if(end==1)
            break;
        fileInBlock=0;
    }

    if(end!=1)
    {
        struct addressBlock* directBlock=(struct addressBlock*)(fileSystem+(s->firstBlockAdd)+(s->blockSize)*(parent->singleAdd[0]));
        for(int i=0; i<100; i++)
        {
            struct file* f=(struct file*)(fileSystem+(s->firstBlockAdd)+(s->blockSize)*(directBlock->directAdd[i]));
            while(fileInBlock!=maxFileInBlock)
            {
                if(f->iNodeNo==deletingInode)
                {
                    struct file* lastFile=findReplacementFile(parent, s);
                    strcpy(f->fileName, lastFile->fileName);
                    f->iNodeNo=lastFile->iNodeNo;
                    end=1;
                    break;
                }
                f++;
                fileInBlock++;
            }
            if(end==1)
                break;
            fileInBlock=0;
        }
    }

    if(end!=1)
    {
        struct singleLink* singleBlock=(struct singleLink*)(fileSystem+(s->firstBlockAdd)+(s->blockSize)*(parent->doubleAdd[0]));
        for(int j=0; j<100; j++)
        {
            struct addressBlock* directBlock=(struct addressBlock*)(fileSystem+(s->firstBlockAdd)+(s->blockSize)*(singleBlock->singleAdd[j]));
            for(int i=0; i<100; i++)
            {
                struct file* f=(struct file*)(fileSystem+(s->firstBlockAdd)+(s->blockSize)*(directBlock->directAdd[i]));
                while(fileInBlock!=maxFileInBlock)
                {
                    if(f->iNodeNo==deletingInode)
                    {
                        struct file* lastFile=findReplacementFile(parent, s);
                        strcpy(f->fileName, lastFile->fileName);
                        f->iNodeNo=lastFile->iNodeNo;
                        end=1;
                        break;
                    }
                    f++;
                    fileInBlock++;
                }
                if(end==1)
                    break;
                fileInBlock=0;
            }
            if(end==1)
                break;
        }
    }

    if(end!=1)
    {
        struct doubleLink* doubleBlock=(struct doubleLink*)(fileSystem+(s->firstBlockAdd)+(s->blockSize)*(parent->tripleAdd[0]));
        for(int k=0; k<100; k++)
        {
            struct singleLink* singleBlock=(struct singleLink*)(fileSystem+(s->firstBlockAdd)+(s->blockSize)*(doubleBlock->doubleAdd[k]));
            for(int j=0; j<100; j++)
            {
                struct addressBlock* directBlock=(struct addressBlock*)(fileSystem+(s->firstBlockAdd)+(s->blockSize)*(singleBlock->singleAdd[j]));
                for(int i=0; i<100; i++)
                {
                    struct file* f=(struct file*)(fileSystem+(s->firstBlockAdd)+(s->blockSize)*(directBlock->directAdd[i]));
                    while(fileInBlock!=maxFileInBlock)
                    {
                        if(f->iNodeNo==deletingInode)
                        {
                            struct file* lastFile=findReplacementFile(parent, s);
                            strcpy(f->fileName, lastFile->fileName);
                            f->iNodeNo=lastFile->iNodeNo;
                            end=1;
                            break;
                        }
                        f++;
                        fileInBlock++;
                    }
                    if(end==1)
                        break;
                    fileInBlock=0;
                }
                if(end==1)
                    break;
            }

            if(end==1)
                break;
        }
    }

    parent->size-=sizeof(struct file);
    parent->modificationDate=time(NULL);
}

void removeDirectory(char* directoryPath, struct superblock* s, struct iNode* iN)
{
    //Finding the parents of the new directory
    char parents[s->iNodeCount][1000];
    int directoryCountInPath=0;
    retrieveParents(s->iNodeCount, parents, &directoryCountInPath, directoryPath);

    //Checking validity and finding the directory's iNode
    struct iNode* newInode=NULL;
    int status=checkPathValidity(s->iNodeCount, 1000, parents, directoryCountInPath, s, iN);
    if(status==-2 || status==-1)
    {
        fprintf(stderr, "The given directory does not exist.\n");
        exit(EXIT_FAILURE);
    }

    //Finding it's iNode
    newInode=iN+status;

    if(newInode==NULL)
    {
        fprintf(stderr, "Cannot find the iNode of the directory.\n");
        exit(EXIT_FAILURE);
    }

    //Check if it is a directory
    if(newInode->isFree!=DIR)
    {
        fprintf(stderr, "This is not a directory\n");
        exit(EXIT_FAILURE);
    }

    //Check if the directory is free
    if(newInode->size!=sizeof(struct file)*2)
    {
        fprintf(stderr, "The directory is not empty.\n");
        exit(EXIT_FAILURE);
    }

    //Removing the directory from the parent directory
    struct iNode* parentDirectory=iN+findParentINode(s->iNodeCount, 1000, parents, directoryCountInPath, directoryCountInPath-1, s, iN);
    removeDirFromDir(parentDirectory, status, s);

    //Removing the directory
    strcpy(newInode->name, "NULL");
    newInode->size=0;
    newInode->creationDate=-1;
    newInode->modificationDate=-1;
    newInode->isFree=FREE;

    for(int i=0; i<7; i++)
        if(newInode->directAdd[i]!=-1)
            s->freeBlocks[newInode->directAdd[i]]=FREE;

    if(newInode->singleAdd[0]!=-1)
    {
        struct addressBlock* directBlock=(struct addressBlock*)(fileSystem+(s->firstBlockAdd)+(s->blockSize)*(newInode->singleAdd[0]));
        for(int i=0; i<100; i++)
            if(directBlock->directAdd[i]!=-1)
                s->freeBlocks[directBlock->directAdd[i]]=FREE;

        s->freeBlocks[newInode->singleAdd[0]]=FREE;
    }

    if(newInode->doubleAdd[0]!=-1)
    {
        struct singleLink* singleBlock=(struct singleLink*)(fileSystem+(s->firstBlockAdd)+(s->blockSize)*(newInode->doubleAdd[0]));
        for(int j=0; j<100; j++)
        {
            if(singleBlock->singleAdd[j]!=-1)
            {
                struct addressBlock* directBlock=(struct addressBlock*)(fileSystem+(s->firstBlockAdd)+(s->blockSize)*(singleBlock->singleAdd[j]));
                for(int i=0; i<100; i++)
                if(directBlock->directAdd[i]!=-1)
                    s->freeBlocks[directBlock->directAdd[i]]=FREE;

                s->freeBlocks[singleBlock->singleAdd[j]]=FREE;
            }
            else
                break;
        }

        s->freeBlocks[newInode->doubleAdd[0]]=FREE;
    }

    if(newInode->tripleAdd[0]!=-1)
    {
        struct doubleLink* doubleBlock=(struct doubleLink*)(fileSystem+(s->firstBlockAdd)+(s->blockSize)*(newInode->tripleAdd[0]));
        for(int k=0; k<100; k++)
        {
            if(doubleBlock->doubleAdd[k]!=-1)
            {
                struct singleLink* singleBlock=(struct singleLink*)(fileSystem+(s->firstBlockAdd)+(s->blockSize)*(doubleBlock->doubleAdd[k]));
                for(int j=0; j<100; j++)
                {
                    if(singleBlock->singleAdd[j]!=-1)
                    {
                        struct addressBlock* directBlock=(struct addressBlock*)(fileSystem+(s->firstBlockAdd)+(s->blockSize)*(singleBlock->singleAdd[j]));
                        for(int i=0; i<100; i++)
                        if(directBlock->directAdd[i]!=-1)
                            s->freeBlocks[directBlock->directAdd[i]]=FREE;

                        s->freeBlocks[singleBlock->singleAdd[j]]=FREE;
                    }
                    else
                        break;
                }

                s->freeBlocks[doubleBlock->doubleAdd[k]]=FREE;
            }
        }

        s->freeBlocks[newInode->tripleAdd[0]]=FREE;
    }

    for(int i=0; i<7; ++i)
        newInode->directAdd[i]=-1;
    newInode->singleAdd[0]=-1;
    newInode->doubleAdd[0]=-1;
    newInode->tripleAdd[0]=-1;
}

void printDirectoryInfo(struct superblock* s, struct iNode* iN, char* directoryPath)
{
    //Checking validity and finding the iNode number
    char parents[s->iNodeCount][1000];
    int directoryCountInPath=0;
    retrieveParents(s->iNodeCount, parents, &directoryCountInPath, directoryPath);
    int iNodeNumber=checkPathValidity(s->iNodeCount, 1000, parents, directoryCountInPath, s, iN);

    if(iNodeNumber<0)
    {
        fprintf(stderr, "The directory does not exist.\n");
        exit(EXIT_FAILURE);
    }

    struct iNode* directoryINode=iN+iNodeNumber;

    if(directoryINode->isFree!=DIR)
    {
        fprintf(stderr, "This is not a directory\n");
        exit(EXIT_FAILURE);
    }

    int totalFiles=(directoryINode->size)/sizeof(struct file);
    int filesRead=0;
    int maxFileInBlock=s->blockSize/sizeof(struct file);
    int fileInBlock=0;

    printf("*****Directory Information*****\n");
    printf("Directory Name: %s\n", directoryINode->name);
    printf("Creation Date: %s",  asctime(localtime((&directoryINode->creationDate))));
    printf("Last Modification Date: %s\n", (asctime(localtime((&directoryINode->modificationDate)))));
    printf("Directory Size: %d\n", directoryINode->size);
    printf("Total Files & Directories: %d\n", totalFiles);
    for(int i=0; i<7; i++)
    {
        if(filesRead==totalFiles)
            break;
        struct file* f=(struct file*)(fileSystem+(s->firstBlockAdd)+(s->blockSize*directoryINode->directAdd[i]));
        while(filesRead!=totalFiles && fileInBlock!=maxFileInBlock)
        {
            int isDir=0;
            struct iNode* retrievedIN=iN+(f->iNodeNo);
            if(retrievedIN->isFree==DIR)
                isDir=1;

            if(filesRead!=0 && filesRead!=1 && isDir==1)
                printf("Directory Name: %s, INode: %d\n", f->fileName, f->iNodeNo);
            else if(filesRead!=0 && filesRead!=1 && isDir==0)
                printf("File Name: %s, INode: %d\n", f->fileName, f->iNodeNo);
            else if(filesRead==0)
                printf("Parent Directory Name: %s, INode: %d\n", f->fileName, f->iNodeNo);
            else if(filesRead==1)
                printf("Parent-Parent Directory Name: %s, INode: %d\n", f->fileName, f->iNodeNo);
            filesRead++;
            f++;
            fileInBlock++;
        }
        fileInBlock=0;
    }

    struct addressBlock* block=(struct addressBlock*)(fileSystem+(s->firstBlockAdd)+(s->blockSize*directoryINode->singleAdd[0]));
    for(int i=0; i<100; ++i)
    {
        if(filesRead==totalFiles)
            break;
        struct file* f=(struct file*)(fileSystem+(s->firstBlockAdd)+(s->blockSize*block->directAdd[i]));
        while(filesRead!=totalFiles && fileInBlock!=maxFileInBlock)
        {
            int isDir=0;
            struct iNode* retrievedIN=iN+(f->iNodeNo);
            if(retrievedIN->isFree==DIR)
                isDir=1;

            if((filesRead!=0 || filesRead!=1) && isDir==1)
                printf("Directory Name: %s, INode: %d\n", f->fileName, f->iNodeNo);
            else if((filesRead!=0 || filesRead!=1) && isDir==0)
                printf("File Name: %s, INode: %d\n", f->fileName, f->iNodeNo);

            filesRead++;
            f++;
            fileInBlock++;
        }
        fileInBlock=0;
    }

    struct singleLink* singleBlock=(struct singleLink*)(fileSystem+(s->firstBlockAdd)+(s->blockSize*directoryINode->doubleAdd[0]));
    for(int i=0; i<100; i++)
    {
        if(filesRead==totalFiles)
            break;
        block=(struct addressBlock*)(fileSystem+(s->firstBlockAdd)+(s->blockSize*singleBlock->singleAdd[i]));
        for(int j=0; j<100; j++)
        {
            if(filesRead==totalFiles)
                break;
            struct file* f=(struct file*)(fileSystem+(s->firstBlockAdd)+(s->blockSize*block->directAdd[j]));
            while(filesRead!=totalFiles && fileInBlock!=maxFileInBlock)
            {
                int isDir=0;
                struct iNode* retrievedIN=iN+(f->iNodeNo);
                if(retrievedIN->isFree==DIR)
                    isDir=1;

                if((filesRead!=0 || filesRead!=1) && isDir==1)
                    printf("Directory Name: %s, INode: %d\n", f->fileName, f->iNodeNo);
                else if((filesRead!=0 || filesRead!=1) && isDir==0)
                    printf("File Name: %s, INode: %d\n", f->fileName, f->iNodeNo);

                filesRead++;
                f++;
                fileInBlock++;
            }
            fileInBlock=0;
        }
    }

    struct doubleLink* doubleBlock=(struct doubleLink*)(fileSystem+(s->firstBlockAdd)+(s->blockSize*directoryINode->tripleAdd[0]));
    for(int i=0; i<100; i++)
    {
        if(filesRead==totalFiles)
            break;
        singleBlock=(struct singleLink*)(fileSystem+(s->firstBlockAdd)+(s->blockSize*doubleBlock->doubleAdd[i]));
        for(int j=0; j<100; j++)
        {
            if(filesRead==totalFiles)
                break;
            block=(struct addressBlock*)(fileSystem+(s->firstBlockAdd)+(s->blockSize*singleBlock->singleAdd[j]));
            for(int k=0; k<100; k++)
            {
                if(filesRead==totalFiles)
                    break;
                struct file* f=(struct file*)(fileSystem+(s->firstBlockAdd)+(s->blockSize*block->directAdd[k]));
                while(filesRead!=totalFiles && fileInBlock!=maxFileInBlock)
                {
                    int isDir=0;
                    struct iNode* retrievedIN=iN+(f->iNodeNo);
                    if(retrievedIN->isFree==DIR)
                        isDir=1;

                    if((filesRead!=0 || filesRead!=1) && isDir==1)
                        printf("Directory Name: %s, INode: %d\n", f->fileName, f->iNodeNo);
                    else if((filesRead!=0 || filesRead!=1) && isDir==0)
                        printf("File Name: %s, INode: %d\n", f->fileName, f->iNodeNo);
                    filesRead++;
                    f++;
                    fileInBlock++;
                }
                fileInBlock=0;
            }
        }
    }
}

void writeToFile(char* path, char* fileName, struct superblock* s, struct iNode* iN)
{
    //Check if the file exists
    FILE *writingFile;
    writingFile=fopen(fileName, "rb");
    if(NULL==writingFile)
    {
        perror("Failed to open the file");
        exit(EXIT_FAILURE);
    }

    //Finding the parents of the new file
    char parents[s->iNodeCount][1000];
    int directoryCountInPath=0;
    int iNodeNumber=0;
    retrieveParents(s->iNodeCount, parents, &directoryCountInPath, path);

    //Checking validity and finding a free iNode
    struct iNode* newInode=NULL;
    int status=checkPathValidity(s->iNodeCount, 1000, parents, directoryCountInPath, s, iN);
    if(status==-1)
    {
        fprintf(stderr, "The given path does not exist.\n");
        exit(EXIT_FAILURE);
    }
    else if(status>=0)
    {
        fprintf(stderr, "Given file already exists.\n");
        exit(EXIT_FAILURE);
    }

    struct iNode* iteratorIN=iN;

    for(int i=0; i<s->iNodeCount; ++i)
    {
        if(newInode==NULL && iteratorIN->isFree==FREE)
        {
            newInode=iteratorIN;
            iNodeNumber=i;
            break;
        }
        else
            iteratorIN++;
    }

    if(newInode==NULL)
    {
        fprintf(stderr, "No iNode is available for a new file.\n");
        exit(EXIT_FAILURE);
    }

    //Finding a free block
    int freeBlockNo=findFreeBlock(s);
    if(freeBlockNo==-1)
    {
        fprintf(stderr, "No blocks are available.\n");
        exit(EXIT_FAILURE);
    }

    //Creating iNode for the file
    strcpy(newInode->name, parents[directoryCountInPath-1]);
    newInode->size=sizeof(struct file)*2;
    newInode->creationDate=time(NULL);
    newInode->modificationDate=time(NULL);
    newInode->isFree=FIL;
    for(int i=0; i<7; ++i)
    {
        if(i==0)
            newInode->directAdd[i]=freeBlockNo;
        else
            newInode->directAdd[i]=-1;
    }
    newInode->singleAdd[0]=-1;
    newInode->doubleAdd[0]=-1;
    newInode->tripleAdd[0]=-1;
    s->freeBlocks[freeBlockNo]=FIL;
    struct file* f=(struct file*)(fileSystem+(s->firstBlockAdd)+(s->blockSize)*freeBlockNo);
    f->iNodeNo=findParentINode(s->iNodeCount, 1000, parents, directoryCountInPath, directoryCountInPath-1, s, (struct iNode*)(fileSystem+s->firstINodeAdd));
    strcpy(f->fileName, parents[directoryCountInPath-2]);
    f++;
    f->iNodeNo=findParentINode(s->iNodeCount, 1000, parents, directoryCountInPath, directoryCountInPath-2, s, (struct iNode*)(fileSystem+s->firstINodeAdd));
    if(directoryCountInPath-3!=-1)
        strcpy(f->fileName, parents[directoryCountInPath-3]);
    else
        strcpy(f->fileName, "..");

    //Updating the parent file
    struct iNode* parentDirectory=iN+findParentINode(s->iNodeCount, 1000, parents, directoryCountInPath, directoryCountInPath-1, s, (struct iNode*)(fileSystem+s->firstINodeAdd));
    int blockAddress=getAvailableBlock(parentDirectory, s);
    parentDirectory->size+=sizeof(struct file);
    parentDirectory->modificationDate=time(NULL);
    f=(struct file*)(fileSystem+(s->firstBlockAdd)+blockAddress);
    f->iNodeNo=iNodeNumber;
    strcpy(f->fileName, parents[directoryCountInPath-1]);

    //Writing to the file
    blockAddress=getAvailableBlock(newInode, s);
    uint8_t* newDataAdd=(fileSystem+(s->firstBlockAdd)+blockAddress);
    while(1==fread(newDataAdd, 1, 1, writingFile))
    {
        (newInode->size)++;
        blockAddress=getAvailableBlock(newInode, s);
        newDataAdd=(fileSystem+(s->firstBlockAdd)+blockAddress);
    }
    fclose(writingFile);
}

void readFromFile(char* path, char* fileName, struct superblock* s, struct iNode* iN)
{
    //Check if the file exists
    FILE *writingFile;
    writingFile=fopen(fileName, "wb+");
    if(NULL==writingFile)
    {
        perror("Failed to open the file");
        exit(EXIT_FAILURE);
    }

    //Finding the parents of the new directory
    char parents[s->iNodeCount][1000];
    int directoryCountInPath=0;
    retrieveParents(s->iNodeCount, parents, &directoryCountInPath, path);

    //Checking validity and finding the directory's iNode
    struct iNode* newInode=NULL;
    int status=checkPathValidity(s->iNodeCount, 1000, parents, directoryCountInPath, s, iN);
    if(status==-2 || status==-1)
    {
        fprintf(stderr, "The given file does not exist.\n");
        exit(EXIT_FAILURE);
    }

    //Finding it's iNode
    newInode=iN+status;

    if(newInode==NULL)
    {
        fprintf(stderr, "Cannot find the iNode of the file.\n");
        exit(EXIT_FAILURE);
    }

    if(newInode->isFree!=FIL)
    {
        fprintf(stderr, "This is not a file.\n");
        exit(EXIT_FAILURE);
    }

    int realSize=newInode->size;
    newInode->size=sizeof(struct file)*2;
    int blockAddress=getAvailableBlock(newInode, s);
    uint8_t* newDataAdd=(fileSystem+(s->firstBlockAdd)+blockAddress);
    while(newInode->size!=realSize && 1==fwrite(newDataAdd, 1, 1, writingFile))
    {
        (newInode->size)++;
        blockAddress=getAvailableBlock(newInode, s);
        newDataAdd=(fileSystem+(s->firstBlockAdd)+blockAddress);
    }

    fclose(writingFile);
}

void removeFile(char* path, struct superblock* s, struct iNode* iN)
{
    //Finding the parents of the new file
    char parents[s->iNodeCount][1000];
    int directoryCountInPath=0;
    retrieveParents(s->iNodeCount, parents, &directoryCountInPath, path);

    //Checking validity and finding the file's iNode
    struct iNode* newInode=NULL;
    int status=checkPathValidity(s->iNodeCount, 1000, parents, directoryCountInPath, s, iN);
    if(status==-2 || status==-1)
    {
        fprintf(stderr, "The given file does not exist.\n");
        exit(EXIT_FAILURE);
    }

    //Finding it's iNode
    newInode=iN+status;

    if(newInode==NULL)
    {
        fprintf(stderr, "Cannot find the iNode of the file.\n");
        exit(EXIT_FAILURE);
    }

    //Check if it is a file
    if(newInode->isFree!=FIL)
    {
        fprintf(stderr, "This is not a file\n");
        exit(EXIT_FAILURE);
    }

    //Removing the file from the parent directory
    struct iNode* parentDirectory=iN+findParentINode(s->iNodeCount, 1000, parents, directoryCountInPath, directoryCountInPath-1, s, iN);
    removeDirFromDir(parentDirectory, status, s);

    //Removing the file
    strcpy(newInode->name, "NULL");
    newInode->size=0;
    newInode->creationDate=-1;
    newInode->modificationDate=-1;
    newInode->isFree=FREE;

    for(int i=0; i<7; i++)
        if(newInode->directAdd[i]!=-1)
            s->freeBlocks[newInode->directAdd[i]]=FREE;

    if(newInode->singleAdd[0]!=-1)
    {
        struct addressBlock* directBlock=(struct addressBlock*)(fileSystem+(s->firstBlockAdd)+(s->blockSize)*(newInode->singleAdd[0]));
        for(int i=0; i<100; i++)
            if(directBlock->directAdd[i]!=-1)
                s->freeBlocks[directBlock->directAdd[i]]=FREE;

        s->freeBlocks[newInode->singleAdd[0]]=FREE;
    }

    if(newInode->doubleAdd[0]!=-1)
    {
        struct singleLink* singleBlock=(struct singleLink*)(fileSystem+(s->firstBlockAdd)+(s->blockSize)*(newInode->doubleAdd[0]));
        for(int j=0; j<100; j++)
        {
            if(singleBlock->singleAdd[j]!=-1)
            {
                struct addressBlock* directBlock=(struct addressBlock*)(fileSystem+(s->firstBlockAdd)+(s->blockSize)*(singleBlock->singleAdd[j]));
                for(int i=0; i<100; i++)
                if(directBlock->directAdd[i]!=-1)
                    s->freeBlocks[directBlock->directAdd[i]]=FREE;

                s->freeBlocks[singleBlock->singleAdd[j]]=FREE;
            }
            else
                break;
        }

        s->freeBlocks[newInode->doubleAdd[0]]=FREE;
    }

    if(newInode->tripleAdd[0]!=-1)
    {
        struct doubleLink* doubleBlock=(struct doubleLink*)(fileSystem+(s->firstBlockAdd)+(s->blockSize)*(newInode->tripleAdd[0]));
        for(int k=0; k<100; k++)
        {
            if(doubleBlock->doubleAdd[k]!=-1)
            {
                struct singleLink* singleBlock=(struct singleLink*)(fileSystem+(s->firstBlockAdd)+(s->blockSize)*(doubleBlock->doubleAdd[k]));
                for(int j=0; j<100; j++)
                {
                    if(singleBlock->singleAdd[j]!=-1)
                    {
                        struct addressBlock* directBlock=(struct addressBlock*)(fileSystem+(s->firstBlockAdd)+(s->blockSize)*(singleBlock->singleAdd[j]));
                        for(int i=0; i<100; i++)
                        if(directBlock->directAdd[i]!=-1)
                            s->freeBlocks[directBlock->directAdd[i]]=FREE;

                        s->freeBlocks[singleBlock->singleAdd[j]]=FREE;
                    }
                    else
                        break;
                }

                s->freeBlocks[doubleBlock->doubleAdd[k]]=FREE;
            }
        }

        s->freeBlocks[newInode->tripleAdd[0]]=FREE;
    }

    for(int i=0; i<7; ++i)
        newInode->directAdd[i]=-1;
    newInode->singleAdd[0]=-1;
    newInode->doubleAdd[0]=-1;
    newInode->tripleAdd[0]=-1;
}
