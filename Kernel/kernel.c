#include "gdt.h"
#include "kernelIO.h"
#include "paging.h"
#include <stdarg.h>

/**
 *	Kernel entrypoint
 */
void kernelMain()
{
	VGAInsert("Hello, World!");
    install_gdt();
	VGAInsert("Success!");
    for (;;);
}