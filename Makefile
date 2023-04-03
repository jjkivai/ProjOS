ASM=nasm
CC=gcc


TOOLS_DIR=tools
BUILD_DIR=build

.PHONY: all floppy_image kernel bootloader clean always fat

all: floppy_image fat

#
# Floppy image
#
floppy_image: $(BUILD_DIR)/os_image.img

$(BUILD_DIR)/os_image.img: bootloader kernel
	dd if=/dev/zero of=$(BUILD_DIR)/os_image.img bs=512 count=2880
	mkfs.fat -F 12 -n "PROJECTOS" $(BUILD_DIR)/os_image.img
	dd if=$(BUILD_DIR)/MBR.bin of=$(BUILD_DIR)/os_image.img conv=notrunc
	mcopy -i $(BUILD_DIR)/os_image.img $(BUILD_DIR)/kernel.bin "::kernel.bin"

#
# Bootloader
#
bootloader: $(BUILD_DIR)/MBR.bin

$(BUILD_DIR)/MBR.bin: always
	$(ASM) boot/MBR.asm -f bin -o $(BUILD_DIR)/MBR.bin

#
# Kernel
#
kernel: $(BUILD_DIR)/kernel.bin

$(BUILD_DIR)/kernel.bin: always
	$(ASM) kernel/main.asm -f bin -o $(BUILD_DIR)/kernel.bin

#
# Tools
#
fat: $(BUILD_DIR)/FAT/fat
$(BUILD_DIR)/FAT/fat: always FAT/fat.c
	mkdir -p $(BUILD_DIR)/tools
	$(CC) -g -o $(BUILD_DIR)/tools/fat FAT/fat.c

#
# Always
#
always:
	mkdir -p $(BUILD_DIR)

#
# Clean
#
clean:
	rm -rf $(BUILD_DIR)/*
	rm -rf */*.bin
	rm -rf */*.img
	rm -rf */*.o
	rm -rf *.bin