#pragma once

#include "kheap.h"
#include <stdint.h>

union Page
{
    struct {
        uint32 present    : 1;
        uint32 rw         : 1;
        uint32 user       : 1;
        uint32 accessed   : 1;
        uint32 dirty      : 1;
        uint32 unused     : 7;
        uint32 frame      : 20;
    };
    uint32_t raw_page;
};

struct PageTable 
{
    union Page pages[1024];
} __attribute__((aligned(4096)));

struct PageDirectory
{
    //struct PageTable* tables[1024];
    uint32 tablesPhysical[1024];
} __attribute__((aligned(4096)));

static struct PageTable first_pt;
static struct PageDirectory pd;

/**
 *	TODO: Docs
 */
void InitPages();

/**
 *	TODO: Docs
 */
void PagingEnable();

void PagingDisable();
