bits 32

section .multiboot
        align 4
        dd 0x1BADB002              
        dd 0x00                    
        dd -(0x1BADB002+0x00)

section .text

extern kernelMain

global start
start:
    cli
    mov esp, stack
    call kernelMain
    hlt

section .bss
resb 8192
stack:
