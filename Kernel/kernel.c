#include "gdt.h"
#include "kernelIO.h"
#include "paging.h"
#include <stdarg.h>

/**
 *	Main kernel
 */
void kernelMain()
{
    /* Tests */
    kClearScr();
    kPrintStr("Emeraldix test!",0x0a); // Output message
    VGANewLine();

    kPrintStr("Emeraldix test!",0x04);
    VGANewLine();

    kPrintStr("Emeraldix test!",0x0e);
    VGANewLine();

    kPrintStr("Emeraldix test!",0x0b);
    VGANewLine();

    kPrintStr("Emeraldix test!",0x07);
    VGANewLine();

    install_gdt();
    kPrintStr("Success!", 0x0f);
    for (;;);
}
