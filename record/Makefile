BUILD_DIR?=build
ASM?=nasm

.PHONY: all record clean always

all: record

record: $(BUILD_DIR)/record.bin

$(BUILD_DIR)/record.bin: MBR.asm
	# $(ASM) MBR.asm -f bin -o $(BUILD_DIR)/MBR.bin
	$(ASM) $< -f bin -o $@
	@echo "--> Compiled: " $<


clean:
	rm -rf $(BUILD_DIR)/record.bin
