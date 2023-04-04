BUILD_DIR = $(abspath build)


.PHONY: all os_image kernel bootloader clean always fat

all: os_image fat

os_image: $(BUILD_DIR)/os_image.img
	qemu-system-i386 -drive format=raw,file=$<,index=0,if=floppy

$(BUILD_DIR)/os_image.img: bootloader kernel
	dd if=/dev/zero of=$(BUILD_DIR)/os_image.img bs=512 count=2880
	mkfs.fat -F 12 -n "PROJECTOS" $(BUILD_DIR)/os_image.img
	dd if=$(BUILD_DIR)/record.bin of=$(BUILD_DIR)/os_image.img conv=notrunc
	# cat $(BUILD_DIR)/sector.bin $(BUILD_DIR)/kernel.bin > $@
	mcopy -i $(BUILD_DIR)/os_image.img $(BUILD_DIR)/sector.bin "::sector.bin"
	mcopy -i $(BUILD_DIR)/os_image.img $(BUILD_DIR)/kernel.bin "::kernel.bin"

bootloader: record sector

record: $(BUILD_DIR)/record.bin

$(BUILD_DIR)/record.bin: always
	@echo "--> Compiled record"
	@$(MAKE) -C record BUILD_DIR=$(abspath $(BUILD_DIR))


sector: $(BUILD_DIR)/sector.bin

$(BUILD_DIR)/sector.bin: always 
	@echo "--> Compiled sector"
	@$(MAKE) -C sector BUILD_DIR=$(abspath $(BUILD_DIR))

kernel: $(BUILD_DIR)/kernel.bin

$(BUILD_DIR)/kernel.bin: always
	@$(MAKE) -C kernel BUILD_DIR=$(abspath $(BUILD_DIR))


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

clean:
	$(MAKE) -C record clean BUILD_DIR=$(abspath $(BUILD_DIR))
	$(MAKE) -C sector clean BUILD_DIR=$(abspath $(BUILD_DIR))
	$(MAKE) -C kernel clean BUILD_DIR=$(abspath $(BUILD_DIR))
	rm -rf $(BUILD_DIR)/*