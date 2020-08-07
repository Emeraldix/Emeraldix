#pragma once

// Only for starting paging

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;

uint32 kmalloc(uint32 sz);
uint32 kmalloc_a(uint32 sz, char align);