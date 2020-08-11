#include "gdt.h"
#include "idt.h"
#include "kernelIO.h"
#include "stdlib.h"
#include "paging.h"
#include <stdarg.h>

extern void isr0(void);
extern void isr14(void);

/**
 *  Main kernel
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
    install_gate(0, isr0, 0x8f, 0x08);
    install_gate(14, isr14, 0x8f, 0x08);
    int a = 2 / 0;
    
    KInit();
    
    int b = *(int*)(0x12345678);
    kPrintStr("Success!\n", 0x0f);
    
    char* hh = (char*)malloc(sizeof(char) * 6);
    
    if(hh)
    {
        hh = "Test\n";
        kPrintStr(hh, 0x0f);
    }

    for (;;);
}
