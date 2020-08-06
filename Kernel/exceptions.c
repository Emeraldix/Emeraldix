#include "kernelIO.h"
#include "paging.h"

void exception0_handler(uint32_t num, uint32_t* eip) {
    kPrintStr("Division by zero occurred\n", 0x2);
    kPrintChar(num + '0', 0x2);
    VGANewLine();
    *eip += 2; //bc division by zero pushes the eip of the div instruction
}

void exception4_handler(uint32_t num) {
    kPrintStr("An exception occurred\n", 0x2);
    kPrintChar(num + '0', 0x2);
    VGANewLine();
}

void exception14_handler(uint32_t num) {
    kPrintStr("A page fault occured\n", 0x2);
    kPrintChar(num + '0', 0x2);
    VGANewLine();
    PagingDisable();
}
