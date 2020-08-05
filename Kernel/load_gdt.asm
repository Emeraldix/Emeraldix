gdtr: dw 0
      dd 0

global load_gdt
load_gdt:
    mov eax, [esp + 4]
    mov [gdtr + 2], eax
    mov ax, [esp + 8]
    mov [gdtr], ax
    lgdt [gdtr]
    ret
