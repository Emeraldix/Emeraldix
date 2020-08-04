#pragma once

// Only for starting paging

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;

uint32 kmalloc_a(uint32 sz, int align);
uint32 kmalloc_p(uint32 sz, uint32 *phys);
uint32 kmalloc_ap(uint32 sz, int align, uint32 *phys);
uint32 kmalloc(uint32 sz);