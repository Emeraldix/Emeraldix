BITS 16
org 0x7C00
BS_jmpBoot:
	jmp short start
	nop

BS_OEMName  	db 'Emeraldi'
BPB_BytsPerSec  dw 0
BPB_SecPerClus  db 0
BPB_RsvdSecCnt  dw 0
BPB_NumFATs		db 0
BPB_RootEntCnt  dw 0
BPB_TotSec16    dw 0
BPB_Media   	db 0
BPB_FATsz16 	dw 0
BPB_SecPerTrk   dw 0
BPB_NumHeads    dw 0
BPB_HiddSec 	dd 0
BPB_TotSec32    dd 0

EBR_SecPerFat   	dd 0
EBR_Flags			dw 0
EBR_FatVersion		dw 0
EBR_ClusRootDir		dd 0
EBR_SecNumFSInfo	dw 1 ; Желательно, иногда жестко забит на 1 сектор
EBR_SecNumBackup	dw 6 ; Желательно, в программах восстановления иногда забит именно 6
resb 12 ; EBR_Reserved
EBR_Drive			db 0
resb 1 ;  EBR_Reserved flags in Windows NT
EBR_Signature		db 0
EBR_VolumeID		dd 0
resb 11 ; EBR_VolumeLabel
EBR_SysIdent		db 'FAT32   '

strlocat db 0 ; номер строки 

start:
	cli

	mov ax, cs
	mov ds, ax
	mov es, ax
	mov ss, ax
	mov sp, 0x7C00
	
	sti
	
	; Установка видео режима
	mov ah, 0x00
	mov al, 0x01
	int 0x10
	call clear

	jmp main
	
; Минималистичная консоль для вывода
; in: color bl, sizec cx, str [bp] - печатает строку
write_str:
	mov ah, 0x13
	mov al, 0x01
	mov dh, byte[strlocat]
	mov dl, 0
	int 0x10
	cmp byte[strlocat], 0x18
	je .sdv
	inc byte[strlocat]
	ret
	
.sdv:
	mov ah, 0x06
	mov al, 0x01
	mov ch, 0
	mov cl, 0
	mov dh, 0x18
	mov dl, 79
	int 0x10
	ret

; in: void - очистка экрана
clear:
	mov ah, 0x06
	mov al, 0
	mov ch, 0
	mov cl, 0
	mov dh, 0x18
	mov dl, 79
	int 0x10
	mov byte[strlocat], 0
	ret

; in: sizec cx, str [bp] - печатает строку с ошибкой
write_error:
	mov bl, 0xc
	call write_str
	ret

; in: secnum ax, count cx, dist [di] - загружает сектор в память
load_sector:
	push dx
	push si
	mov byte[0x600], 0x10
	mov byte[0x601], 0
	mov word[0x602], cx
	mov [0x604], di
	push es
	pop word[0x606]
	mov [0x608], ax
	mov [0x60A], dx
	mov word[0x60C], 0
	mov word[0x60E], 0
	mov ah, 0x42
	;mov dl, byte[EBR_Drive] - когда будет заполнен BPB
	mov dl, 80h
	mov si, 0x600
	int 0x13
	jc .error
	pop si
	pop dx
	ret
.error:
	mov cx, 10
	mov bp, ReadError
	call write_error
	pop si
	pop dx
	ret

main:
	xor dx, dx
	mov ax, 1
	mov cx, 4
	mov di, 0x7E00
	call load_sector
	
	jmp 0x7E00

ReadError db 'Read error'

resb 510 - ($ - $$)
db 0x55,0xAA

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