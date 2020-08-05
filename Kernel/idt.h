#pragma once
#include <stdint.h>

struct idt_entry {
    uint32_t offset;
    uint16_t selector;
    uint8_t type;
};

struct raw_idt_entry {
    uint16_t offset_1;
    uint16_t selector;
    uint8_t zero;
    uint8_t type;
    uint16_t offset_2;
};

static struct idt_entry idt[128] = {
    {0, 0, 0}
};

static struct raw_idt_entry raw_idt[128];

extern void load_idt(char* idt, int size);
void encode_idt(struct idt_entry entry, struct raw_idt_entry* target);
void install_idt(void);
void install_gate(char num, void (*handler)(void), char type, short sel);
