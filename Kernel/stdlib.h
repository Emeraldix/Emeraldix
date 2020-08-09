#pragma once

void memcpy(void *dest, const void *src, unsigned int len);
void memset(void *dest, unsigned char val, unsigned int len);

char *itoa(int number, char *destination, int base);

typedef unsigned int size_t;

union header {
	struct {
		size_t size;
		char isFree;
		union header* next;
	} inf;
	char allign[16];
};

void* malloc(size_t size);
void* calloc(size_t num, size_t nsize);
void* realloc(void *block, size_t size);
void free(void *block);