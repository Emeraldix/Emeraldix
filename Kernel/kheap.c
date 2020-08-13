#include "kheap.h"

extern uint32 end;
uint32 placement_address = (uint32)&end;

uint32 kmalloc(uint32 sz)
{
    uint32 tmp = placement_address;
    placement_address += sz;
    return tmp;
}

uint32 kmalloc_a(uint32 sz, char align)
{
    if(align && (placement_address & 0xFFF))
    {
        placement_address &= 0xFFFFF000;
        placement_address += 0x1000;
    }
    uint32 tmp = placement_address;
    placement_address += sz;
    return tmp;
}

