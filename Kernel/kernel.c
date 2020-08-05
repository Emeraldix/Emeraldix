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
    
    VGASetCursor(30,5);
    kPrintStr("Emeraldix test!",0x0a); // Output message
    VGANewLine();

    kPrintStr("Emeraldix test!",0x04);
    VGANewLine();

    kPrintStr("Emeraldix test!",0x0e);

    VGASetCursor(30,8);
    kPrintStr("Emeraldix test!",0x0b);

    VGASetCursor(30,9);
    kPrintStr("Emeraldix test!",0x07);

    install_gdt();
    kPrintStr("Success!", 0x0f);
    for (;;);
}
