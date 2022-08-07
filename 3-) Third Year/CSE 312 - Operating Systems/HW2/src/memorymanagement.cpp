
#include <memorymanagement.h>

using namespace myos;
using namespace myos::common;

void printf(char* str);
void printfHex(uint8_t);

MemoryManager* MemoryManager::activeMemoryManager = 0;
MemoryManager2* MemoryManager2::activeMemoryManager = 0;

//Initializes the RAM
//Allocates the very first chunk. Sets it's pageFrame number as 0.
//The first chunk's size takes the whole RAM space until new chunks are put into it.
//Eventually all chunks will have the same size (20*1024)
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
        totalFrames=0;
        first -> frameNo=totalFrames;
    }
}

//Initializes the RAM
//Allocates the very first chunk. Sets it's pageFrame number as 0.
//The first chunk's size takes the whole RAM space until new chunks are put into it.
//Eventually all chunks will have the same size (20*1024)
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
        totalFrames=0;
        first -> frameNo=totalFrames;
    }
}

//Initializes the Disk
//Allocates the very first chunk. Sets it's pageFrame number as 0.
//The first chunk's size takes the whole Disk space until new chunks are put into it.
//Eventually all chunks will have the same size (20*1024)
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
        totalFrames=1024;
        first -> frameNo=totalFrames;
    }
}

//Initializes the Disk
//Allocates the very first chunk. Sets it's pageFrame number as 0.
//The first chunk's size takes the whole Disk space until new chunks are put into it.
//Eventually all chunks will have the same size (20*1024)
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

//Allocates space for a new memory chunk in the RAM
//If there is no space left, returns 0.
//If there is space, returns a new block address
//Sets the new block's frame number
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

//Allocates space for a new memory chunk in the Disk
//If there is no space left, returns 0.
//If there is space, returns a new block address
//Sets the new block's frame number
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
//Initializes the MMU
//Allocates space for RAM and Disk according to the given sizes.
//If the given sizes does not fit to the OS, it uses default sizes = 4MB Ram, 16MB Disk
//It sets the default block sizes as (20*1024)
//It calculates how many blocks the ram and the disk can hold and sets the page tables according to this number
//This is necessary because the page table must have pages exactly the same amount as ram/disk max block count
MMU::MMU(size_t ramSize, size_t diskSize, uint32_t memupper, uint32_t algorithm)
{
    size_t startRam=10*1024*1024;
    size_t endRam=startRam+ramSize*1024*1024;
    size_t startDisk=endRam+1*1024*1024;
    size_t endDisk=startDisk+diskSize*1024*1024;
    totalCounter=0;
    hitCounter=0;
    missCounter=0;
    resetRefCounter=0;
    totalMiss=0;
    totalHit=0;


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

MMU::MMU()
{

}

//0 if fifo, 1 if chance, 2 if lru
//ramSize and diskSize in MB
//Initializes the MMU
//Allocates space for RAM and Disk according to the given sizes.
//If the given sizes does not fit to the OS, it uses default sizes = 4MB Ram, 16MB Disk
//It sets the default block sizes as (20*1024)
//It calculates how many blocks the ram and the disk can hold and sets the page tables according to this number
//This is necessary because the page table must have pages exactly the same amount as ram/disk max block count
//Sets the page replacement algorithm that was chosen
void MMU::setMMU(size_t ramSize, size_t diskSize, uint32_t memupper, uint32_t algorithm)
{
    size_t startRam=10*1024*1024;
    size_t endRam=startRam+ramSize*1024*1024;
    size_t startDisk=endRam+1*1024*1024;
    size_t endDisk=startDisk+diskSize*1024*1024;
    totalCounter=0;
    hitCounter=0;
    missCounter=0;
    resetRefCounter=0;
    totalMiss=0;
    totalHit=0;

    if(algorithm==0)
    {
        fifo=1;
        chance=0;
        lru=0;
    }
    else if(algorithm==1)
    {
        fifo=0;
        chance=1;
        lru=0;
    }
    else
    {
        fifo=0;
        chance=0;
        lru=1;
    }

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

//Inserts the given item into the storage
//If there is space in RAM, inserts into RAM
//If RAM is full, inserts into Disk
//If both is full, returns -1
//If insertion is completed, returns 0
//totalCounter is incremented as a counter to be used if lru algorithm is used
//resetRefCounter is incremented to clear referenced bits when needed.
//referenced bits are cleared whenever resetRefCounter reaches 10 and the counter restarts.
uint32_t MMU::insertItem(uint32_t item)
{
    totalCounter++;
    resetRefCounter++;

    if(resetRefCounter==10)
    {
        resetRefCounter=0;
        totalMiss+=missCounter;
        totalHit+=hitCounter;
        missCounter=0;
        hitCounter=0;

        for(int i=0; i<ram.ramPageTable.fullPagesC; ++i)
            ram.ramPageTable.pages[i].referenced=0;
    }

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
        ram.ramPageTable.pages[ram.ramPageTable.fullPagesC].no=totalCounter;
        ram.ramPageTable.pages[ram.ramPageTable.fullPagesC].referenced=1;
        ram.ramPageTable.pages[ram.ramPageTable.fullPagesC].counter=totalCounter;
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

//Returns the item which is registered with the given virtual address.
//If not found, returns -1.
//Firstly searches in RAM. If not found in RAM, looks up in Disk. It means there is a page fault.
//If a page fault happens, after retrieving the frame from the disk, does page replacement.
//totalCounter is incremented as a counter to be used if lru algorithm is used.
//resetRefCounter is incremented to clear referenced bits when needed.
//referenced bits are cleared whenever resetRefCounter reaches 10 and the counter restarts.
uint32_t MMU::getItem(uint32_t virtualAddress)
{
    totalCounter++;
    resetRefCounter++;

    if(resetRefCounter==10)
    {
        printf("\nTotal Misses Current Second: ");
        printfHex(missCounter);
        printf("\nTotal Hits Current Second: ");
        printfHex(hitCounter);
        printf("\nMiss Rate Current Second: %");
        int misses=missCounter;
        int hits=hitCounter;
        int hitRate=(100*hits)/(misses+hits);
        int missRate=(100*misses)/(misses+hits);
        printfHex(missRate);
        printf("\nHit Rate Current Second: %");
        printfHex(hitRate);
        printf("\nPages Loaded & Pages Written Back Current Second: ");
        printfHex(misses);
        printf("\n");

        resetRefCounter=0;
        totalMiss+=missCounter;
        totalHit+=hitCounter;
        missCounter=0;
        hitCounter=0;

        for(int i=0; i<ram.ramPageTable.fullPagesC; ++i)
            ram.ramPageTable.pages[i].referenced=0;
    }

    int pageFault=0;
    int faultedDiskIndex=-1;
    uint32_t result=-1;

    //Page table lookup in ram
    uint32_t searchedFrame=-1;
    for(int i=0; i<ram.ramPageTable.fullPagesC; ++i)
        if(ram.ramPageTable.pages[i].index==virtualAddress)
        {
            searchedFrame=ram.ramPageTable.pages[i].frameNo;
            ram.ramPageTable.pages[i].referenced=1;
            ram.ramPageTable.pages[i].counter=totalCounter;
        }

    //Page Fault, lookup in disk
    if(searchedFrame==-1)
    {
        pageFault=1;
        missCounter++;
        for(int i=ram.ramPageTable.pageC; i<disk.diskPageTable.fullPagesC+ram.ramPageTable.pageC; ++i)
            if(disk.diskPageTable.pages[i].index==virtualAddress)
            {
                searchedFrame=disk.diskPageTable.pages[i].frameNo;
                disk.diskPageTable.pages[i].referenced=1;
                disk.diskPageTable.pages[i].counter=totalCounter;
                faultedDiskIndex=i;
            }
    }
    else
        hitCounter++;

    int ramBlockCount=ram.ramPageTable.pageC;
    int diskBlockCount=disk.diskPageTable.pageC;

    //Retrieving the frame
    MemoryChunk* start=ram.first;
    for(int i=0; i<ramBlockCount; ++i)
    {
        if(start->frameNo==searchedFrame)
            result= (*((uint32_t*)(((size_t)start) + sizeof(MemoryChunk))));
        start=start->next;
    }

    if(result==-1)
    {
        MemoryChunk* start=disk.first;
        for(int i=0; i<diskBlockCount; ++i)
        {
            if(start->frameNo==searchedFrame)
                result= (*(uint32_t*)(((size_t)start) + sizeof(MemoryChunk)));
            start=start->next;
        }
    }

    if(result==-1)
        return -1;

    //DO PAGE REPLACEMENT HERE
    if(faultedDiskIndex!=-1)
    {
        int replacingIndex=-1;
        if(fifo==1)
            replacingIndex=fifoOperation();
        else if(chance==1)
            replacingIndex=chanceOperation();
        else
            replacingIndex=lruOperation();

        if(replacingIndex==-1)
            return -1;

        pageReplacementOperation(faultedDiskIndex, replacingIndex);
    }

    return result;
}

//Finds a page to replace in ram according to FIFO algorithm
//no in pages in ram page table indicates when they entered ram
//smaller no means earlier entrance therefore the smallest one should be removed
uint32_t MMU::fifoOperation()
{
    int minVal=ram.ramPageTable.pages[0].no;
    int resultIndex=0;

    for(int i=0; i<ram.ramPageTable.fullPagesC; ++i)
        if(ram.ramPageTable.pages[i].no<minVal)
        {
            minVal=ram.ramPageTable.pages[i].no;
            resultIndex=i;
        }

    return resultIndex;
}

int isInArray(int arraySize, int* arr, int item)
{
    for(int i=0; i<arraySize; ++i)
        if(arr[i]==item)
            return 1;

    return 0;
}

//Finds a page to replace in ram according to Second Chance algorithm
//no in pages in ram page table indicates when they entered ram
//smaller no means earlier entrance therefore the smallest one should be removed
//However, if this page has it's referenced bit as 1 it's referenced bit is cleaned, then another page is searched
uint32_t MMU::chanceOperation()
{
    int minVal=ram.ramPageTable.pages[0].no;
    int resultIndex=0;
    int prevArray[ram.ramPageTable.fullPagesC];
    int counter=0;

    while(1)
    {
        for(int i=0; i<ram.ramPageTable.fullPagesC; ++i)
        {
            if(ram.ramPageTable.pages[i].no<minVal && !(counter!=ram.ramPageTable.fullPagesC && isInArray(ram.ramPageTable.fullPagesC, prevArray, i)))
            {
                minVal=ram.ramPageTable.pages[i].no;
                resultIndex=i;
            }
        }

        if(ram.ramPageTable.pages[resultIndex].referenced==1)
        {
            ram.ramPageTable.pages[resultIndex].referenced=0;
            prevArray[counter]=resultIndex;
            counter++;
        }
        else
            break;
    }

    return resultIndex;
}

//Finds a page to replace in ram according to LRU algorithm
//counter in pages in ram page table indicates when they were last referenced
//smaller counter means they were referenced earlier
//Therefore the page with the smallest counter must be removed
uint32_t MMU::lruOperation()
{
    int minVal=ram.ramPageTable.pages[0].counter;
    int resultIndex=0;

    for(int i=0; i<ram.ramPageTable.fullPagesC; ++i)
        if(ram.ramPageTable.pages[i].counter<minVal)
        {
            minVal=ram.ramPageTable.pages[i].counter;
            resultIndex=i;
        }

    return resultIndex;
}

//Does the page replacement operation between the given diskIndex and the ramIndex
void MMU::pageReplacementOperation(common::uint32_t diskIndex, common::uint32_t ramIndex)
{
    int tempIndex=ram.ramPageTable.pages[ramIndex].index;
    int tempFrameNo=ram.ramPageTable.pages[ramIndex].frameNo;
    int tempNo=ram.ramPageTable.pages[ramIndex].no;
    int tempReferenced=ram.ramPageTable.pages[ramIndex].referenced;
    int tempCounter=ram.ramPageTable.pages[ramIndex].counter;

    ram.ramPageTable.pages[ramIndex].index=disk.diskPageTable.pages[diskIndex].index;
    ram.ramPageTable.pages[ramIndex].frameNo=disk.diskPageTable.pages[diskIndex].frameNo;
    ram.ramPageTable.pages[ramIndex].no=disk.diskPageTable.pages[diskIndex].no;
    ram.ramPageTable.pages[ramIndex].referenced=disk.diskPageTable.pages[diskIndex].referenced;
    ram.ramPageTable.pages[ramIndex].counter=disk.diskPageTable.pages[diskIndex].counter;

    disk.diskPageTable.pages[diskIndex].index=tempIndex;
    disk.diskPageTable.pages[diskIndex].frameNo=tempFrameNo;
    disk.diskPageTable.pages[diskIndex].no=tempNo;
    disk.diskPageTable.pages[diskIndex].referenced=tempReferenced;
    disk.diskPageTable.pages[diskIndex].counter=tempCounter;

    ram.ramPageTable.pages[ramIndex].no=totalCounter;
}

//If a pageFrame's content is changed, does the necessary adjustments
void MMU::changeVirtualAddress(uint32_t address, uint32_t otherAddress)
{
    int changed=0;
    //Search in RAM
    for(int i=0; i<ram.ramPageTable.fullPagesC; ++i)
    {
        if(ram.ramPageTable.pages[i].index==address)
        {
            ram.ramPageTable.pages[i].index=otherAddress;
            changed++;
        }
        else if(ram.ramPageTable.pages[i].index==otherAddress)
        {
            ram.ramPageTable.pages[i].index=address;
            changed++;
        }
    }

    //Search in Disk
    if(changed!=2)
    {
        for(int i=0; i<disk.diskPageTable.fullPagesC; ++i)
        {
            if(disk.diskPageTable.pages[i].index==address)
            {
                disk.diskPageTable.pages[i].index=otherAddress;
                changed++;
            }
            else if(disk.diskPageTable.pages[i].index==otherAddress)
            {
                disk.diskPageTable.pages[i].index=address;
                changed++;
            }
        }
    }

}

//Initializes the page table.
//pageC means how many pages it can hold at max
//disk pages start from where ram pages are left therefore extra is calculated for indexing
PageTable::PageTable(uint32_t pageCount, uint16_t isDisk, uint32_t ramPageCount)
{
    pageC=pageCount;
    int extra=0;
    if(isDisk==1)
        extra=ramPageCount;

    for(int i=0+extra; i<pageCount+extra; ++i)
    {
        struct Page newPage;
        newPage.index=i;
        newPage.frameNo=-1;
        newPage.no=i;
        newPage.referenced=0;
        newPage.counter=0;
        pages[i]=newPage;
    }
}

//Initializes the page table.
//pageC means how many pages it can hold at max
//disk pages start from where ram pages are left therefore extra is calculated for indexing
void PageTable::setTable(uint32_t pageCount, uint16_t isDisk,uint32_t ramPageCount)
{
    pageC=pageCount;
    int extra=0;
    if(isDisk==1)
        extra=ramPageCount;

    fullPagesC=0+extra;

    for(int i=0+extra; i<pageCount+extra; ++i)
    {
        struct Page newPage;
        newPage.index=i;
        newPage.frameNo=-1;
        newPage.no=i;
        newPage.referenced=0;
        newPage.counter=0;
        pages[i]=newPage;
    }
}

PageTable::~PageTable()
{

}
