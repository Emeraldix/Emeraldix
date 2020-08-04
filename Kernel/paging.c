#include "paging.h"

struct PageDirectory* dir;

void InitPages()
{
	
}

void OnPaging()
{
	__asm__ volatile ("mov %0, %%cr3"::"r"(&dir->tablesPhysical));
    uint32 cr0;
    __asm__ volatile ("mov %%cr0, %0": "=r"(cr0));
    cr0 |= 0x80000000;
    __asm__ volatile ("mov %0, %%cr0":: "r"(cr0));
}