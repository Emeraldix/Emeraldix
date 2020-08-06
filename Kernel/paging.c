#include "paging.h"

void InitPages()
{
    for (int i = 0; i < 1024; i++) {
        pd.tablesPhysical[i] = 2;
        first_pt.pages[i].raw_page = (i * 0x1000) | 3;
    }
    pd.tablesPhysical[0] = ((uint32_t)first_pt.pages)| 3;
}

void PagingEnable()
{
    __asm__ volatile ("mov %0, %%cr3"::"r"(&pd));
    uint32 cr0;
    __asm__ volatile ("mov %%cr0, %0": "=r"(cr0));
    cr0 |= 0x80000000;
    __asm__ volatile ("mov %0, %%cr0":: "r"(cr0));
}

void PagingDisable()
{
    uint32 cr0;
    __asm__ volatile ("mov %%cr0, %0": "=r"(cr0));
    cr0 &= ~0x80000000;
    __asm__ volatile ("mov %0, %%cr0":: "r"(cr0));
}
