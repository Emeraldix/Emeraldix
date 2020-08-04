#include "kheap.h"

extern uint32 end;
uint32 placement_address = (uint32)&end;

uint32 kmalloc(uint32 sz)
{
    uint32 tmp = placement_address;
    placement_address += sz;
    return tmp;
}

uint32 kmalloc_a(uint32 sz, int align)
{
    if (align && (placement_address & 0xFFFFF000))
    {
        placement_address &= 0xFFFFF000;
        placement_address += 0x1000;
    }
    uint32 tmp = placement_address;
    placement_address += sz;
    return tmp;
}

uint32 kmalloc_p(uint32 sz, uint32 *phys)
{
    uint32 tmp = placement_address;
	if(phys)
    {
        *phys = placement_address;
    }
    placement_address += sz;
    return tmp;
}

uint32 kmalloc_ap(uint32 sz, int align, uint32 *phys)
{
    if (align && (placement_address & 0xFFFFF000))
    {
        placement_address &= 0xFFFFF000;
        placement_address += 0x1000;
    }
    if(phys)
    {
        *phys = placement_address;
    }
    uint32 tmp = placement_address;
    placement_address += sz;
    return tmp;
}