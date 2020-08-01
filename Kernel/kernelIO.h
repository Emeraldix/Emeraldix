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
 * Выводит сообщение на экран
 * @param msg Выводимое сообщение
 */
void VGAInsert(const char* msg)
{
	char* videoBuff = (char*) VIDEOMEM;

	unsigned int i = 0;
	unsigned int j = 0;

	while(msg[j] != '\0') /* Считываем до конца строки */
	{
		videoBuff[i+1] = 0x07; /* Сообщаем черный цвет */
		videoBuff[i] = msg[j]; /* Вводим символ */
		++j; /* Счетчик для считывания символов из 'messageOutput' */
		i = i + 2; /* Cчетчик для видеобуффера */
	}
}

/**
 *	Устанавливает курсор
 *	@param x Положение курсора по оси X
 *	@param y Положение курсора по оси Y
 */
void VGASetCursor(unsigned int x,unsigned int y);
