#define VIDEOMEM 0xb8000

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
		videoBuff[i] = 0x07; /* Сообщаем черный цвет */
		videoBuff[i+1] = msg[j]; /* Вводим символ */
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