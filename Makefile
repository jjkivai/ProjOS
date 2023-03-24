# $@ = target file
# $< = first dependency
# $^ = all dependencies

# detect all .o files based on their .c source
C_SOURCES = $(wildcard *.c drivers/*.c cpu/*.c)
HEADERS = $(wildcard *.h  drivers/*.h cpu/*.h)
OBJ_FILES = ${C_SOURCES:.c=.o cpu/interrupt.o}

# First rule is the one executed when no parameters are fed to the Makefile
all: run

# Notice how dependencies are built as needed
kernel.bin: kernel-entry.o ${OBJ_FILES}
	ld -m elf_i386 -o $@ -Ttext 0x1000 $^ --oformat binary

os-image.bin: mbr.bin kernel.bin
	cat $^ > $@

run: os-image.bin
	qemu-system-i386 -fda $<

echo: os-image.bin
	xxd $<

# only for debug
kernel.elf: boot/kernel_entry.o ${OBJ_FILES}
	ld -m elf_i386 -o $@ -Ttext 0x1000 $^


%.o: %.c ${HEADERS}
	gcc -m32 -ffreestanding -fno-pie -c $< -o $@ # -g for debugging

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -o $@

%.dis: %.bin
	ndisasm -b 32 $< > $@

clean:
	$(RM) *.bin *.o *.dis *.elf
	$(RM) kernel/*.o
	$(RM) boot/*.o boot/*.bin
	$(RM) drivers/*.o
	$(RM) cpu/*.o
