#include "stdlib.h"
#include "paging.h"

void memcpy(void *dest, const void *src, unsigned int len)
{
    const unsigned char *sp = (const unsigned char*)src;
    unsigned char *dp = (unsigned char*)dest;
    while(len--) 
        *dp++ = *sp++;
}

void memset(void *dest, unsigned char val, unsigned int len)
{
    unsigned char *temp = (unsigned char*)dest;
    while(len--) 
        *temp++ = val;
}

char *itoa(int number, char *destination, int base) {
    int count = 0;
    do {
        int digit = number % base;
        destination[count++] = (digit > 9) ? digit - 10 +'A' : digit + '0';
    } while ((number /= base) != 0);
    destination[count] = '\0';
    int i;
   
    for (i = 0; i < count / 2; ++i) 
    {
        char symbol = destination[i];
        destination[i] = destination[count - i - 1];
        destination[count - i - 1] = symbol;
    }
    
    return destination;
}

union header *head = 0, *tail = 0; 
void* firstPtr = 0; size_t pos = 0;

union header* getFreeBlock(size_t size)
{
    union header* curr = head;
    while(curr) 
    {
        if(curr->inf.isFree && curr->inf.size >= size)
            return curr;
        curr = curr->inf.next;
    }
    
    return 0;
}

void* malloc(size_t size)
{
    size_t total_size;
    void* block;
    union header* header;
    
    if(!size || size > 4096 - sizeof(union header))
        return 0;
    
    // lock mutex
    header = getFreeBlock(size);
	if(header) 
    {
        header->inf.isFree = 0;
        // unlock mutex
        return (void*)(header + 1);
    }
    
    total_size = sizeof(union header) + size;
	
    if(!firstPtr || pos + total_size > 4096)
    {
        if(pos + total_size > 4096 && 4096 - pos > sizeof(union header))
        {
            union header* res = ((union header*)malloc(4096 - pos - sizeof(union header)) - 1);
            res->inf.isFree = 1;
        }
        firstPtr = AllocPage(1); // kernel
        if(!firstPtr)
        {
            // unlock mutex
            return 0;
        }
        
        block = firstPtr;
        pos = total_size;
    }
    else
    {
        block = (firstPtr + pos);
        pos += total_size;
    }
    
	header = block;
	header->inf.size = size;
	header->inf.isFree = 0;
	header->inf.next = 0;
    
    if(!head)
        head = header;
    
    if(tail)
        tail->inf.next = header;
    tail = header;
    // unlock mutex
    
    return (void*)(header + 1);
}

void* calloc(size_t num, size_t nsize)
{
    size_t size;
    void* block;
    
    if(!num || !nsize)
        return 0;
    
    size = num * nsize;
    if(nsize != size / num)
        return 0;
    
    block = malloc(size);
    if(!block)
        return 0;
    memset(block, 0, size);
    
    return block;
}

void* realloc(void *block, size_t size)
{
    union header* header;
	void* ret;
    
	if(!block || !size)
		return malloc(size);
    
	header = (union header*)block - 1;
	if(header->inf.size >= size)
		return block;
    
	ret = malloc(size);
	if(ret) 
    {
		memcpy(ret, block, header->inf.size);
		free(block);
	}
    
	return ret;
}

void free(void *block)
{
    union header *header, *tmp;
    void* programbreak;

    if(!block)
        return;
    
    // lock mutex
    header = (union header*)block - 1;
    header->inf.isFree = 1;
    // unlock mutex
}