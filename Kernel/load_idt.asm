idtr: dw 0
      dd 0

global load_idt
load_idt:
    mov eax, [esp + 4]
    mov [idtr + 2], eax
    mov ax, [esp + 8]
    mov [idtr], ax
    lidt [idtr]
    ret

; test
global isr0
isr0:
    pusha
    mov eax, 0xb8000
    mov byte[eax], 'a'
    mov byte[eax + 1], 0xf
    popa
    iret
