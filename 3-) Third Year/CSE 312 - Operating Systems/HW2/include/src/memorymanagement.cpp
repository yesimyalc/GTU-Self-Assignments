
#include <memorymanagement.h>

using namespace myos;
using namespace myos::common;

void printf(char* str);
void printfHex(uint8_t);

MemoryManager* MemoryManager::activeMemoryManager = 0;
MemoryManager2* MemoryManager2::activeMemoryManager = 0;

MemoryManager::MemoryManager(size_t start, size_t size)
{
    activeMemoryManager = this;

    if(size < sizeof(MemoryChunk))
    {
        first = 0;
    }
    else
    {
        first = (MemoryChunk*)start;

        first -> allocated = false;
        first -> prev = 0;
        first -> next = 0;
        first -> size = size - sizeof(MemoryChunk);
        totalFrames=1;
        first -> frameNo=totalFrames;
    }
}

void MemoryManager::setManager(size_t start, size_t size)
{
    activeMemoryManager = this;

    if(size < sizeof(MemoryChunk))
    {
        first = 0;
    }
    else
    {
        first = (MemoryChunk*)start;

        first -> allocated = false;
        first -> prev = 0;
        first -> next = 0;
        first -> size = size - sizeof(MemoryChunk);
        totalFrames=1;
        first -> frameNo=totalFrames;
    }
}

void MemoryManager2::setManager(size_t start, size_t size)
{
    activeMemoryManager = this;

    if(size < sizeof(MemoryChunk))
    {
        first = 0;
    }
    else
    {
        first = (MemoryChunk*)start;

        first -> allocated = false;
        first -> prev = 0;
        first -> next = 0;
        first -> size = size - sizeof(MemoryChunk);
        totalFrames=1;
        first -> frameNo=totalFrames;
    }
}

MemoryManager2::MemoryManager2(size_t start, size_t size)
{
    activeMemoryManager = this;

    if(size < sizeof(MemoryChunk))
    {
        first = 0;
    }
    else
    {
        first = (MemoryChunk*)start;

        first -> allocated = false;
        first -> prev = 0;
        first -> next = 0;
        first -> size = size - sizeof(MemoryChunk);
        totalFrames=1024;
        first -> frameNo=totalFrames;
    }
}

PageTable::PageTable()
{

}

MemoryManager::MemoryManager()
{

}

MemoryManager2::MemoryManager2()
{

}

MemoryManager::~MemoryManager()
{
    if(activeMemoryManager == this)
        activeMemoryManager = 0;
}

MemoryManager2::~MemoryManager2()
{
    if(activeMemoryManager == this)
        activeMemoryManager = 0;
}

void* MemoryManager::malloc(size_t size)
{
    MemoryChunk *result = 0;

    for(MemoryChunk* chunk = first; chunk != 0 && result == 0; chunk = chunk->next)
        if(chunk->size > size && !chunk->allocated)
            result = chunk;

    if(result == 0)
        return 0;

    if(result->size >= size + sizeof(MemoryChunk) + 1)
    {
        MemoryChunk* temp = (MemoryChunk*)((size_t)result + sizeof(MemoryChunk) + size);

        temp->allocated = false;
        temp->size = result->size - size - sizeof(MemoryChunk);
        temp->prev = result;
        temp->next = result->next;
        if(temp->next != 0)
            temp->next->prev = temp;

        result->size = size;
        result->next = temp;
    }

    result->allocated = true;
    result -> frameNo=totalFrames;
    totalFrames++;
    return (void*)(((size_t)result) + sizeof(MemoryChunk));
}

void* MemoryManager2::malloc(size_t size)
{
    MemoryChunk *result = 0;

    for(MemoryChunk* chunk = first; chunk != 0 && result == 0; chunk = chunk->next)
        if(chunk->size > size && !chunk->allocated)
            result = chunk;

    if(result == 0)
        return 0;

    if(result->size >= size + sizeof(MemoryChunk) + 1)
    {
        MemoryChunk* temp = (MemoryChunk*)((size_t)result + sizeof(MemoryChunk) + size);

        temp->allocated = false;
        temp->size = result->size - size - sizeof(MemoryChunk);
        temp->prev = result;
        temp->next = result->next;
        if(temp->next != 0)
            temp->next->prev = temp;

        result->size = size;
        result->next = temp;
    }

    result->allocated = true;
    result -> frameNo=totalFrames;
    totalFrames++;
    return (void*)(((size_t)result) + sizeof(MemoryChunk));
}

void MemoryManager::free(void* ptr)
{
    MemoryChunk* chunk = (MemoryChunk*)((size_t)ptr - sizeof(MemoryChunk));

    chunk -> allocated = false;

    if(chunk->prev != 0 && !chunk->prev->allocated)
    {
        chunk->prev->next = chunk->next;
        chunk->prev->size += chunk->size + sizeof(MemoryChunk);
        if(chunk->next != 0)
            chunk->next->prev = chunk->prev;

        chunk = chunk->prev;
    }

    if(chunk->next != 0 && !chunk->next->allocated)
    {
        chunk->size += chunk->next->size + sizeof(MemoryChunk);
        chunk->next = chunk->next->next;
        if(chunk->next != 0)
            chunk->next->prev = chunk;
    }

}

void MemoryManager2::free(void* ptr)
{
    MemoryChunk* chunk = (MemoryChunk*)((size_t)ptr - sizeof(MemoryChunk));

    chunk -> allocated = false;

    if(chunk->prev != 0 && !chunk->prev->allocated)
    {
        chunk->prev->next = chunk->next;
        chunk->prev->size += chunk->size + sizeof(MemoryChunk);
        if(chunk->next != 0)
            chunk->next->prev = chunk->prev;

        chunk = chunk->prev;
    }

    if(chunk->next != 0 && !chunk->next->allocated)
    {
        chunk->size += chunk->next->size + sizeof(MemoryChunk);
        chunk->next = chunk->next->next;
        if(chunk->next != 0)
            chunk->next->prev = chunk;
    }

}


//ramSize and diskSize in MB
MMU::MMU(size_t ramSize, size_t diskSize, uint32_t memupper)
{
    size_t startRam=10*1024*1024;
    size_t endRam=startRam+ramSize*1024*1024;
    size_t startDisk=endRam+1*1024*1024;
    size_t endDisk=startDisk+diskSize*1024*1024;

    //Checking if disk exceeds OS limit
    if(endDisk>=memupper)
    {
        printf("\nGiven sizes are too big for the OS. The default sizes of 4MB ram, 16MB disk will be used instead.\n");
        endRam=startRam+4*1024*1024;
        startDisk=endRam+1*1024*1024;
        endDisk=startDisk+16*1024*1024;
    }

    //Initializing ram and disk spaces
    ram.setManager(startRam, endRam);
    disk.setManager(startDisk, endDisk);

    //Initializing Page Tables inside both ram and disk space
    int blockSize=20*1024;
    int ramBlockCount=(ramSize*1024*1024)/blockSize;
    int diskBlockCount=(diskSize*1024*1024)/blockSize;

    ram.ramPageTable.setTable(ramBlockCount, 0, ramBlockCount);
    disk.diskPageTable.setTable(diskBlockCount, 1, ramBlockCount);

}

MMU::~MMU()
{

}

uint32_t MMU::insertItem(uint32_t item)
{
    if((ram.ramPageTable.fullPagesC)==(ram.ramPageTable.pageC) && (disk.diskPageTable.fullPagesC)==(disk.diskPageTable.pageC))
        return -1;
    else if((ram.ramPageTable.fullPagesC)<(ram.ramPageTable.pageC))
    {
        //Inserting into ram
        int blockSize=20*1024;
        uint32_t* blockAddress=(uint32_t*)ram.malloc(blockSize);
        (*blockAddress)=item;

        //Registering the address into virtual memory
        MemoryChunk* blockChunk=(MemoryChunk*)((size_t)blockAddress-sizeof(MemoryChunk));
        ram.ramPageTable.pages[ram.ramPageTable.fullPagesC].frameNo=blockChunk->frameNo;
        ram.ramPageTable.fullPagesC++;

    }
    else
    {
        //Inserting into disk
        int blockSize=20*1024;
        uint32_t* blockAddress=(uint32_t*)disk.malloc(blockSize);
        (*blockAddress)=item;

        //Registering the address into virtual memory
        MemoryChunk* blockChunk=(MemoryChunk*)((size_t)blockAddress-sizeof(MemoryChunk));
        disk.diskPageTable.pages[disk.diskPageTable.fullPagesC].frameNo=blockChunk->frameNo;
        disk.diskPageTable.fullPagesC++;

    }

    return 0;
}

uint32_t MMU::getItem(uint32_t virtualAddress)
{
    int pageFault=0;
    //Page table lookup in ram
    uint32_t searchedFrame=-1;
    for(int i=0; i<ram.ramPageTable.fullPagesC; ++i)
        if(ram.ramPageTable.pages[i].index==virtualAddress)
            searchedFrame=ram.ramPageTable.pages[i].frameNo;

    //Page Fault, lookup in disk
    if(searchedFrame==-1)
    {
        pageFault=1;
        for(int i=ram.ramPageTable.pageC; i<disk.diskPageTable.fullPagesC+ram.ramPageTable.pageC; ++i)
            if(disk.diskPageTable.pages[i].index==virtualAddress)
                searchedFrame=disk.diskPageTable.pages[i].frameNo;
    }

    int ramBlockCount=ram.ramPageTable.pageC;
    int diskBlockCount=disk.diskPageTable.pageC;

    //Retrieving the frame
    if(pageFault==0)
    {
        MemoryChunk* start=ram.first;
        for(int i=0; i<ramBlockCount; ++i)
        {
            if(start->frameNo==searchedFrame)
                return (*((uint32_t*)(((size_t)start) + sizeof(MemoryChunk))));
            start=start->next;
        }
    }
    else
    {
        MemoryChunk* start=disk.first;
        for(int i=0; i<diskBlockCount; ++i)
        {
            if(start->frameNo==searchedFrame)
                return (*(uint32_t*)(((size_t)start) + sizeof(MemoryChunk)));
            start=start->next;
        }
    }

    return -1;
}

PageTable::PageTable(uint32_t pageCount, uint16_t isDisk, uint32_t ramPageCount)
{
    pageC=pageCount;
    fullPagesC=0;
    int extra=0;
    if(isDisk==1)
        extra=ramPageCount;

    for(int i=0+extra; i<pageCount+extra; ++i)
    {
        struct Page newPage;
        newPage.index=i;
        newPage.frameNo=-1;
        pages[i]=newPage;
    }
}

void PageTable::setTable(uint32_t pageCount, uint16_t isDisk,uint32_t ramPageCount)
{
    pageC=pageCount;
    fullPagesC=0;
    int extra=0;
    if(isDisk==1)
        extra=ramPageCount;

    for(int i=0+extra; i<pageCount+extra; ++i)
    {
        struct Page newPage;
        newPage.index=i;
        newPage.frameNo=-1;
        pages[i]=newPage;
    }
}

PageTable::~PageTable()
{

}
