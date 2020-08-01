#include "kernelIO.h"
#include <stdarg.h>

/**
 *	Точка входа в ядро.
 */
void kernelMain()
{
	VGAInsert("Hello, World!"); // Выводим сообщение
	return;
}