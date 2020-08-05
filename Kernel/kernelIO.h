#define VIDEOMEM 0xb8000

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


/**
 * Put a message on the screen
 * @param msg The message
 */
void VGAInsert(const char* msg);

/**
 *	Set cursor's coords
 *	@param x X coordinate
 *	@param y Y coordinate
 */
void VGASetCursor(unsigned int x,unsigned int y);
