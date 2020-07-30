all: boot.img
	
boot.img: boot.asm FatCreator
	nasm -f bin boot.asm -o boot.bin
	./FatCreator disk 50 $@
	./FatCreator fat32 $@ boot.bin

#TODO: make a better system 
FatCreator:
	cmake -S ../Utilities/FatCreator -B ../Utilities/FatCreator
	make -C ../Utilities/FatCreator
	cp ../Utilities/FatCreator/FatCreator .

test: boot.img
	qemu-system-x86_64 -hda $<
