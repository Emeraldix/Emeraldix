C_FILES := $(wildcard *.c)
ASM_FILES := $(wildcard *.asm)
O_FILES := $(C_FILES:.c=.o)
O_FILES += $(ASM_FILES:.asm=.o)

.PHONY: clean test

all: build kernel

kernel: $(O_FILES) link.ld
	ld -T link.ld -o build/$@ build/*.o -m elf_i386

%.o: %.c
	gcc -o build/$@ -c $< -ffreestanding -nostdlib -m32

%.o: %.asm
	nasm -f elf32 -o build/$@ $<

build:
	mkdir -p build

test: kernel
	qemu-system-x86_64 -kernel build/$<

clean:
	rm -rf build
