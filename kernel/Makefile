BUILD_DIR?=build
ASM?=nasm

.PHONY: all kernel clean

all: kernel

kernel: $(BUILD_DIR)/kernel.bin

$(BUILD_DIR)/kernel.bin:
	@$(ASM) main.asm -f bin -o $@

clean:
	@rm -rf $(BUILD_DIR)/kernel.bin