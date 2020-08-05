#pragma once

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;

/**
 *	Pushes a 8 bit value to I / O.
 *	@param toPort I / O port
 *	@param val Port value
 */
static inline void outb(uint16_t toPort, uint8_t val)
{
    __asm__ volatile ( "outb %0, %1" : : "a"(val), "Nd"(toPort) );
}

/**
 *	Receives an 8 bit value from an I / O port
 *	@param outPort Source
 *	@return uint8_t 8 bit value 
 */
static inline uint8_t inb(uint16_t outPort)
{
    uint8_t ret;
    __asm__ volatile ( "inb %1, %0"
            : "=a"(ret)
            : "Nd"(outPort) );
    
    return ret;
}
