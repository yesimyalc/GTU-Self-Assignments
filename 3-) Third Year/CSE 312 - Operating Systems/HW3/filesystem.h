#ifndef FILESYSTEM_H_INCLUDED
#define FILESYSTEM_H_INCLUDED

#define KB_1 (1024)
#define MB_1 (1024*KB_1)
#define FREEMAPSIZE 16*MB_1/KB_1

#define FREE 1
#define FIL 0
#define DIR 2

struct superblock{
    uint16_t blockSize;     //will be X KB
    uint16_t blockCount;
    uint32_t firstBlockAdd;
    uint8_t freeBlocks[FREEMAPSIZE];
    uint16_t iNodeCount;
    uint32_t firstINodeAdd; //Root
}__attribute__ ((__packed__)) superblock;

struct file{
    uint16_t iNodeNo;
    char fileName[14];  //Max 13 characters name
}__attribute__ ((__packed__)) file;

struct iNode{
    char name[14];
    uint32_t size;
    time_t creationDate;
    time_t modificationDate;
    uint8_t isFree;
    int16_t directAdd[7];
    int16_t singleAdd[1];
    int16_t doubleAdd[1];
    int16_t tripleAdd[1];
}__attribute__ ((__packed__)) iNode;

struct addressBlock{
    int16_t directAdd[100];
}__attribute__ ((__packed__)) addressBlock;

struct singleLink{
    int16_t singleAdd[100];
}__attribute__ ((__packed__)) singleLink;

struct doubleLink{
    int16_t doubleAdd[100];
}__attribute__ ((__packed__)) doubleLink;

uint8_t fileSystem[16*MB_1]={0};

#endif // FILESYSTEM_H_INCLUDED
