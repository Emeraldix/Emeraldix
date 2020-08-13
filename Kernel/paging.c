#include "paging.h"

extern uint32 placement_address;

static uint32 pframeUp = 0; // Atomic...
static uint32 pframeLimit = 0;
static uint32* pframe = 0;

static char AllocFrame(uint32 *frame)
{
    if(pframeUp > 0 && frame)
    {
        *frame = pframe[--pframeUp];
    }
    else // Memory out or invalid frame
        return 0;
}

static void FreeFrame(uint32 frame)
{
    if(pframeUp < pframeLimit)
        pframe[pframeUp++] = frame;
}

void KInit()
{
    // Need call int 15 to load memmap
    uint32 memcount = 16384 * 0x1000; // 64 MB
    pframe = (uint32*)kmalloc(sizeof(uint32) * memcount/0x1000);
    pframeLimit = memcount/0x1000;
    
    kdir = (struct PageDirectory*)kmalloc_a(sizeof(struct PageDirectory), 1);
    currdir = kdir;
    
    for(uint32 i = 0; i < 1024; ++i)
    {
        GetPage(i * 1024, 1, 0x7, 1);
    }
    
    uint32 i = 0;
    while(i < placement_address + 1)
    {
        struct Page* p = GetPage(i/0x1000, 1, 0x7, 1);
        p->present = 1; p->rw = 1; p->user = 0;
        p->frame = i/0x1000;
        
        i += 0x1000;
    }
    
    while(i < memcount)
    {
        FreeFrame(i/0x1000);
        i += 0x1000;
    }
    SwitchPageDir(kdir);
}

void InitPages(struct PageDirectory* dir, void* ptr, size_t pages)
{
    struct PageDirectory* tmp = currdir;
    currdir = dir;
    GetPage(0, 1, 0x7, 0);
    currdir = tmp;
    
    uint32 opptr = (uint32)ptr/0x1000;
    uint32 lastAlloc = 0;
    for(uint32 i = 0; i < pages; ++i)
    {
        uint32 table_idx = i / 1024;
        if(table_idx > lastAlloc)
        {
            tmp = currdir;
            currdir = dir;
            GetPage(table_idx * 0x1000, 1, 0x7, 0);
            currdir = tmp;
        }
        uint32 table_idy = i % 1024;
        struct Page* p = &dir->tables[table_idx]->pages[table_idy];
        p->user = 1; p->present = 1; p->rw = 1;
        p->frame = GetPage(opptr + i, 0, 0, 0)->frame;
    }
}

void SwitchPageDir(struct PageDirectory* dir)
{
    uint32* tmp = &dir->tablesPhysical;
    if(dir != kdir)
        tmp = GetPage((uint32)&dir->tablesPhysical/0x1000, 0, 0, 0)->frame * 0x1000;
    currdir = dir;
    __asm__ volatile ("mov %0, %%cr3"::"r"(tmp));
    uint32 cr0;
    __asm__ volatile ("mov %%cr0, %0": "=r"(cr0));
    cr0 |= 0x80000000;
    __asm__ volatile ("mov %0, %%cr0":: "r"(cr0));
}

struct Page* GetPage(uint32 index, char make, uint32 flags, char kernel)
{
    uint32 table_idx = index / 1024;
    uint32 table_idy = index % 1024;
    if (currdir->tables[table_idx])
        return &currdir->tables[table_idx]->pages[table_idy];
    else if(make)
    {
        uint32 tmp;
        if(kernel)
        {
            currdir->tables[table_idx] = kmalloc_a(sizeof(struct PageTable), 1);
            tmp = currdir->tables[table_idx];
        }
        else
        {
            struct PageDirectory* dir = currdir;
            currdir = kdir;
            void* tmp_k = aligned_alloc(0x1000, sizeof(struct PageTable));
            if(!tmp_k)
            {
                currdir = dir;
                return 0;
            }
            currdir = dir;
            currdir->tables[table_idx] = tmp_k;
            tmp = GetPage((uint32)currdir->tables[table_idx]/0x1000, 0, 0, 0)->frame * 0x1000;
        }
        memset(currdir->tables[table_idx], 0, 0x1000);
        currdir->tablesPhysical[table_idx] = tmp | flags;
        
        return &currdir->tables[table_idx]->pages[table_idy];
    }
    else
        return 0;
}

void* AllocPage(char kernel)
{
    uint32 index = 0;
    struct Page* p = 0;
    for(uint32 i = 0; i < 1024 * 1024; ++i)
    {
        p = GetPage(i, 1, 0x7, 0);
        if(p->present)
            p = 0;
        else
        {
            index = i;
            break;
        }
    }
    
    if(!p)
        return 0;
    
    uint32 frame;
    if(!AllocFrame(&frame))
        return 0;
    
    if(kernel)
        p->user = 0;
    else
        p->user = 1;
    p->present = 1; p->rw = 1;
    p->frame = frame;
    
    return index * 0x1000;
}

void FreePage(void* firstInd)
{
    struct Page* p = GetPage((uint32)firstInd/0x1000, 0, 0, 0);
    
    if(p)
        if(p->present)
        {
            FreeFrame(p->frame);
            memset(p, 0, sizeof(struct Page));
        }
}

void PagingDisable()
{
    uint32 cr0;
    __asm__ volatile ("mov %%cr0, %0": "=r"(cr0));
    cr0 &= ~0x80000000;
    __asm__ volatile ("mov %0, %%cr0":: "r"(cr0));
}
