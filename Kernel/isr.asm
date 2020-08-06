saved_eip: dd 0

%macro isr 1
global isr%1
isr%1:
    mov dword[saved_eip], esp
    pusha
    mov eax, dword[saved_eip]
    push eax
    mov eax, %1
    push eax
    extern exception%1_handler
    call exception%1_handler
    pop eax
    pop eax
    popa
    iret
%endmacro

%macro isr_error 1
global isr%1
isr%1:
    mov dword[saved_eip], esp
    pusha
    mov eax, dword[saved_eip]
    push eax
    mov eax, %1
    push eax
    extern exception%1_handler
    call exception%1_handler
    pop eax
    pop eax
    popa
    add esp, 4 ; skip the error code
    iret
%endmacro

isr 0
isr 4
isr_error 14
