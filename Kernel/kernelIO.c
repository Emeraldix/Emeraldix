#include "kernelIO.h"
#include "ioaccess.h"

void kClearScr(void) {
    char* ptr = (char*)VIDEOMEM;
    for (int i = 0; i < VGA_WIDTH*VGA_HEIGHT*2; i++) {
        ptr[i] = 0;
    }
    VGASetCursor(cur_x, cur_y);
}

void kPrintStr(const char* msg,unsigned char color)
{
    char* videoBuff = (char*) VIDEOMEM + (cur_y * 80 + cur_x) * 2;

    unsigned int j = 0;

    while(msg[j] != '\0') 
    {
        videoBuff[j * 2] = msg[j];
        videoBuff[j * 2 + 1] = color; 
        ++j; 
    }

    cur_x += j;
    if (cur_x >= VGA_WIDTH) {
        cur_x %= 25;
        cur_y++;
    }
    if (cur_y >= VGA_HEIGHT) cur_y = 0;
    VGASetCursor(cur_x, cur_y);
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

void VGANewLine()
{
    cur_x = 0;
    if(++cur_y >= VGA_HEIGHT) cur_y = 0;
    VGASetCursor(cur_x, cur_y);
}
