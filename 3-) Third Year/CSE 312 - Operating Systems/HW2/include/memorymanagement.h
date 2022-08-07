
#ifndef __MYOS__MEMORYMANAGEMENT_H
#define __MYOS__MEMORYMANAGEMENT_H

#include <common/types.h>


namespace myos
{
    /*Memory chunk is used as blocks inside ram/disk.
    It includes necessary info*/
    struct MemoryChunk
    {
        MemoryChunk *next;
        MemoryChunk *prev;
        bool allocated;
        common::uint32_t frameNo;
        common::size_t size;
    };

    /*Page is used inside page tables.
    index is for the virtual address
    frameNo is for the physical address
    no is for the insertion time of the page into the page table (used for FIFO)
    referenced is 1 if referenced recently, 0 if not
    counter is for the last reference time (used for LRU)*/
    struct Page{
        int index;
        int frameNo;
        int no;
        int referenced;
        int counter;
    };

    /*Page table is to construct the conversion between virtual memory and physical memory
    pageC is the total pages it can store
    fullPagesC is the amount it currently stores*/
    class PageTable
    {
    public:
        PageTable(common::uint32_t pageCount, common::uint16_t isDisk, common::uint32_t ramPageCount);
        PageTable();
        void setTable(common::uint32_t pageCount, common::uint16_t isDisk, common::uint32_t ramPageCount);
        ~PageTable();

        common::uint32_t pageC;
        common::uint32_t fullPagesC;

        Page pages[4096];
    };

    /*Memory manager represents RAM.
    It has it's own page table
    malloc is done thanks to MemoryChunks
    At the start of the program, RAM block size is defined so that when RAM is full,
    it is divided into equal chunks. This is necessary for the page table to work properly*/
    class MemoryManager
    {

    protected:
        common::uint32_t totalFrames;
    public:
        MemoryChunk* first;
        PageTable ramPageTable;
        static MemoryManager *activeMemoryManager;

        MemoryManager(common::size_t start, common::size_t size);
        MemoryManager();
        void setManager(common::size_t start, common::size_t size);
        ~MemoryManager();

        void* malloc(common::size_t size);
        void free(void* ptr);
    };

    /*This class represents Disk.
    It functions the same way as RAM however it is bigger and RAM is preferred more*/
    class MemoryManager2
    {

    protected:
        common::uint32_t totalFrames;
    public:
        MemoryChunk* first;
        PageTable diskPageTable;
        static MemoryManager2 *activeMemoryManager;

        MemoryManager2(common::size_t start, common::size_t size);
        MemoryManager2();
        void setManager(common::size_t start, common::size_t size);
        ~MemoryManager2();

        void* malloc(common::size_t size);
        void free(void* ptr);
    };

    /*MMU controls both RAM and Disk.
    Whenever something stored is wanted to be reached, MMU does virtual address to physical address translation and retrieves the item.
    It also does item insertions.
    If page faults occurs while looking up for an item, the MMU does page replacements.
    The MMU is responsible of clearing/setting referenced bits, last referenced dates etc*/
    class MMU
    {
    public:
        int totalCounter;
        int hitCounter;
        int missCounter;
        int totalHit;
        int totalMiss;
        int resetRefCounter;

        int fifo;
        int chance;
        int lru;

        MemoryManager ram;
        MemoryManager2 disk;
        MMU(common::size_t ramSize, common::size_t diskSize, common::uint32_t memupper, common::uint32_t algorithm);
        MMU();
        void setMMU(common::size_t ramSize, common::size_t diskSize, common::uint32_t memupper, common::uint32_t algorithm);
        ~MMU();

        common::uint32_t insertItem(common::uint32_t item);
        common::uint32_t getItem(common::uint32_t virtualAddress);
        common::uint32_t fifoOperation();
        common::uint32_t chanceOperation();
        common::uint32_t lruOperation();
        void pageReplacementOperation(common::uint32_t diskIndex, common::uint32_t ramIndex);
        void changeVirtualAddress(common::uint32_t address, common::uint32_t otherAddress);
    };
}

#endif
