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
	VGAInsert("Emeraldix test!",0x0a); // Output message

	VGASetCursor(30,6);
	VGAInsert("Emeraldix test!",0x04);

	VGASetCursor(30,7);
	VGAInsert("Emeraldix test!",0x0e);

	VGASetCursor(30,8);
	VGAInsert("Emeraldix test!",0x0b);

	VGASetCursor(30,9);
	VGAInsert("Emeraldix test!",0x07);

    install_gdt();
	VGAInsert("Success!", 0x0f);
    for (;;);
}
