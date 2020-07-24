Bits 16
org 0x7E00

stage2:
	; Перед прочитать файл,
	lgdt [gdtr32]
	
	cli
	; -> PM
	mov eax, cr0
	or al, 1
	mov cr0, eax
	
	jmp 8:stage32
	
path_kernel db '/SYS        /KERNEL  BIN'
	
align 16
gdt32:
	dq 0                  ; NULL - 0
	dq 0x00CF9A000000FFFF ; CODE - 8
	dq 0x00CF92000000FFFF ; DATA - 16
gdtr32:
	dw $ - gdt32 - 1
	dd gdt32

BITS 32

stage32:
	mov eax, 16
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	movzx esp, sp
	
	jmp next ; Bochs не поддерживает A20
	
	; Проверка доступности A20
	pushad
	mov edi,0x112345
	mov esi,0x012345
	mov [esi],esi
	mov [edi],edi
	cmpsd
	popad
	je next
	
	; Если доступно включаем
	cli
 
	call    a20wait
	mov     al,0xAD
	out     0x64,al
 
	call    a20wait
	mov     al,0xD0
	out     0x64,al
 
	call    a20wait2
	in      al,0x60
	push    eax
 
	call    a20wait
	mov     al,0xD1
	out     0x64,al
 
	call    a20wait
	pop     eax
	or      al,2
	out     0x60,al
 
	call    a20wait
	mov     al,0xAE
	out     0x64,al
 
	call    a20wait
	
	sti
next:
	; Передать управление
	jmp $
	
a20wait:
	in al,0x64
	test al,2
	jnz a20wait
	ret
 
 
a20wait2:
	in al,0x64
	test al,1
	jz a20wait2
	ret