#pragma once
#define VIDEOMEM 0xb8000

// temp
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

//Colors
#define BLACK {0, 0, 0}
#define RED {255, 0, 0}
#define YELLOW {255, 255, 0}
#define GREEN {0,  255, 0}
#define CYAN {0, 255, 255}
#define BLUE {0, 0, 255}
#define MAGENTA {255, 0, 255}
#define WHITE {255, 255, 255}
#define EMERALD {11, 179, 141}

typedef unsigned short uint16_t;

/**
 * Displays a message on the screen
 * @param msg What should be output
 */
void VGAInsert(const char* msg,unsigned char color);

/**
 *	Set cursor position
 *	@param x X-axis cursor position
 *	@param y Y-axis cursor position
 */
void VGASetCursor(unsigned int x,unsigned int y);

/**
 *	Get cursor position
 *	@return Cursor position
 */
uint16_t VGAGetCursorPosition();

/**
 *	New line
 */
void VGANewLine();

/**
 *	@param Cursor position
 *	@return X-axis cursor position
 */
inline uint16_t getXCursor(uint16_t pos)
{
    return pos % VGA_WIDTH;
}

/**
 *	@param Cursor position
 *	@return Y-axis cursor position
 */
inline uint16_t getYCursor(uint16_t pos)
{
    return pos / VGA_WIDTH;
}
