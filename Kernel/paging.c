#include "paging.h"
#include "stdlib.h"

extern uint32 placement_address;

static uint32 pframeUp = 0;
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
    uint32 i = 0;
    while(i < placement_address + 1)
    {
        struct Page* p = GetPage(i/0x1000, 1, kdir, 0x7, 1);
        p->present = 1; p->rw = 1; p->user = 0;
        p->frame = i/0x1000;
        
        i += 0x1000;
    }
    
    while(i < memcount)
    {
        FreeFrame(i/0x1000);
        i += 0x1000;
    }
}

void InitPages(struct PageDirectory* dir)
{
    // Need malloc
}

void SwitchPageDir(struct PageDirectory* dir)
{
    __asm__ volatile ("mov %0, %%cr3"::"r"(&dir->tablesPhysical));
    uint32 cr0;
    __asm__ volatile ("mov %%cr0, %0": "=r"(cr0));
    cr0 |= 0x80000000;
    __asm__ volatile ("mov %0, %%cr0":: "r"(cr0));
}

struct Page* GetPage(uint32 index, char make, struct PageDirectory* dir, uint32 flags, char kernel)
{
    uint32 table_idx = index / 1024;
    uint32 table_idy = index % 1024;
    if (dir->tables[table_idx])
        return &dir->tables[table_idx]->pages[table_idy];
    else if(make)
    {
        uint32 tmp;
        if(kernel)
        {
            dir->tables[table_idx] = kmalloc_a(sizeof(struct PageTable), 1);
            tmp = dir->tables[table_idx];
        }
        //else
        //  dir->tables[table_idx] = //TODO: malloc & tmp phys address
        memset(dir->tables[table_idx], 0, 0x1000);
        dir->tablesPhysical[table_idx] = tmp | flags;
        
        return &dir->tables[table_idx]->pages[table_idy];
    }
    else
        return 0;
}

void* AllocPage(struct PageDirectory* dir, char kernel)
{
    uint32 index = 0;
    struct Page* p = 0;
    for(uint32 i = 0; i < 1024 * 1024; ++i)
    {
        p = GetPage(i, 1, dir, 0x7, kernel);
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
    
    p->present = 1; p->rw = 1; p->user = 1;
    p->frame = frame;
    
    return index * 0x1000;
}

void FreePage(void* firstInd, struct PageDirectory* dir)
{
    struct Page* p = GetPage((uint32)firstInd/0x1000, 0, dir, 0, 0);
    
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
