#include "kernelIO.h"
#include "ioaccess.h"

void VGAInsert(const char* msg,unsigned char color)
{
	char* videoBuff = (char*) VIDEOMEM;

	unsigned int i = (unsigned int) VGAGetCursorPosition();

	i = i * 2;

	if (i % 2 != 0)
		i++;

	unsigned int j = 0;

	while(msg[j] != '\0') 
	{
		videoBuff[i+1] = color; 
		videoBuff[i] = msg[j];
		++j; 
		i = i + 2;
	}

	i /= 2;

	VGASetCursor(i % VGA_WIDTH,i / VGA_WIDTH);
}

uint16_t VGAGetCursorPosition()
{
	uint16_t pos = 0;
    outb(0x3D4, 0x0F);
    pos |= inb(0x3D5);
    outb(0x3D4, 0x0E);
    pos |= ((uint16_t)inb(0x3D5)) << 8;
    return pos;
}

void VGASetCursor(unsigned int x,unsigned int y)
{
	uint16_t pos = y * VGA_WIDTH + x;
 
	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}
