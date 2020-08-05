#include "idt.h"

void encode_idt(struct idt_entry entry, struct raw_idt_entry* target) {

    target->offset_1 = entry.offset & 0xffff;
    target->offset_2 = entry.offset >> 16 & 0xffff;

    target->selector = entry.selector;
    target->type = entry.type;

    target->zero = 0;
}

void install_idt(void) {
    for (int i = 0; i < 128; i++) {
        encode_idt(idt[i], &raw_idt[i]);
    }
    load_idt((char*)raw_idt, sizeof(raw_idt));
}

void install_gate(char num, void (*handler)(void), char type, short sel) {
    idt[num].offset = handler;
    idt[num].type = type;
    idt[num].selector = sel;
    encode_idt(idt[num], &raw_idt[num]);
}
