# $@ = target file
# $< = first dependency
# $^ = all dependencies

# First rule is the one executed when no parameters are fed to the Makefile
all: run

kernel.bin: kernel-entry.o kernel.o interrupt.o
	ld -m elf_i386 -o $@ -Ttext 0x1000 $^ --oformat binary

kernel-entry.o: kernel-entry.asm
	nasm $< -f elf -o $@

interrupt.o: drivers/interrupt.asm
	nasm $< -f elf -o $@

kernel.o: kernel.c
	gcc -m32 -ffreestanding -fno-pie -c $< -o $@

mbr.bin: mbr.asm
	nasm $< -f bin -o $@


os-image.bin: mbr.bin kernel.bin
	cat $^ > $@

run: os-image.bin
	qemu-system-i386 -drive format=raw,file=$< 
	# qemu-system-i386 -drive format=raw file = $< 
	# qemu-system-x86_64 -fda $<

clean:
	rm *.bin *.o *.dis