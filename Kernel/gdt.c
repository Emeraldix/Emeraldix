#include "gdt.h"
#include "kernelIO.h"

void encode_gdt(struct gdt_entry entry, char* target) {
    if (entry.limit > 65536 && (entry.limit & 0xfff) != 0xfff) {
        VGAInsert("Failed to create a GDT entry", 2);
        for(;;);
    }

    if (entry.limit > 65536) {
        entry.limit >>= 12;
        target[6] = 0xc0;
    }
    else {
        target[6] = 0x40;
    }

    target[0] = entry.limit & 0xff;
    target[1] = entry.limit >> 8 & 0xff;
    target[6] |= entry.limit >> 16 & 0xff;

    target[2] = entry.base & 0xff;
    target[3] = entry.base >> 8 & 0xff;
    target[4] = entry.base >> 16 & 0xff;
    target[7] = entry.base >> 24 & 0xff;

    target[5] = entry.type;
}

void install_gdt(void) {
    for (int i = 0; i < 3; i++) {
        encode_gdt(gdt[i], raw_gdt[i]);
    }
    load_gdt((char*)raw_gdt, sizeof(raw_gdt));
}
