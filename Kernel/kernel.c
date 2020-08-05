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
    kPrintStr("Emeraldix test!\n",0x0a); // Output message
    kPrintStr("Emeraldix test!\n",0x04);
    kPrintStr("Emeraldix test!\n",0x0e);
    kPrintStr("Emeraldix test!\n",0x0b);
    kPrintStr("Emeraldix test!\n",0x07);

    install_gdt();
    kPrintStr("Success!\n", 0x0f);
    for (;;);
}
