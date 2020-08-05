#include "kernelIO.h"

void VGAInsert(const char* msg) {
	char* videoBuff = (char*) VIDEOMEM;

	unsigned int i = 0;
	unsigned int j = 0;

	while(msg[j] != '\0') {
		videoBuff[i] = msg[j];
		videoBuff[i+1] = 0x07;
		++j;
		i = i + 2;
	}
}
