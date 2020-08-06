#include "gdt.h"
#include "idt.h"
#include "kernelIO.h"
#include "paging.h"
#include <stdarg.h>

extern void isr0(void);

/**
 *	Main kernel
 */
void kernelMain()
{
    /* Tests */
    kClearScr();
    for (int i = 1; i < 16; i++)
        kPrintStr("Emeraldix test!\n", i);

    install_gdt();
    kPrintStr("Success!\n", 0x0f);
    install_idt();
    kPrintStr("Success!\n", 0x0f);
    install_gate(0x7f, isr0, 0x8e, 0x08);
    asm("int $0x7f");
    kPrintStr("Success!\n", 0x0f);

    for (;;);
}
