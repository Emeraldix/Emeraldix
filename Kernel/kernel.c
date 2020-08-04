#include "kernelIO.h"
#include "paging.h"
#include <stdarg.h>

/**
 *	Точка входа в ядро.
 */
void kernelMain()
{
	VGAInsert("Hello, World!"); // Выводим сообщение
	return;
}