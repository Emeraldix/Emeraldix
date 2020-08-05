#pragma once
#include <stdint.h>

struct gdt_entry {
    uint32_t base, limit;
    uint8_t type;
};

static struct gdt_entry gdt[] = {
    {0, 0, 0},
    {0, 0xffffffff, 0x9a},
    {0, 0xffffffff, 0x92},
};

static char raw_gdt[3][8];

extern void load_gdt(char* gdt, int size);
void encode_gdt(struct gdt_entry entry, char* target);
void install_gdt(void);
