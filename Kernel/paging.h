#pragma once

#include "kheap.h"
#include <stdint.h>

struct Page
{
    uint32 present    : 1;
    uint32 rw         : 1;
    uint32 user       : 1;
    uint32 accessed   : 1;
    uint32 dirty      : 1;
    uint32 unused     : 7;
    uint32 frame      : 20;
};

struct PageTable
{
    struct Page pages[1024];
};

struct PageDirectory
{
    struct PageTable* tables[1024];
    uint32 tablesPhysical[1024];
};

struct PageDirectory* kdir;
struct PageDirectory* currdir;

void KInit();

void InitPages(struct PageDirectory* dir);
void SwitchPageDir(struct PageDirectory* dir);
struct Page* GetPage(uint32 index, char make, uint32 flags, char kernel);
void* AllocPage(char kernel);
void FreePage(void* firstInd);

void PagingDisable();
