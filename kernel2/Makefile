BUILD_DIR?= build

C_SOURCES := $(wildcard *.cpp */*cpp)
# C_SOURCES += $(wildcard ../drivers/*.cpp)

ASM_SOURCES := $(wildcard *.asm */*asm)
# ASM_SOURCES += $(wildcard ../drivers/*.asm)

C_OBJECTS := $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(C_SOURCES))
ASM_OBJECTS := $(patsubst %.asm, $(BUILD_DIR)/%.o, $(ASM_SOURCES))

HEADERS := $(wildcard *.hpp */*hpp)
# HEADERS += $(wildcard ../drivers/*.hpp)
# OBJ_FILES := ${C_SOURCES:.cpp=$(BUILD_DIR)/%.o}
DRIVERS := $(wildcard $(BUILD_DIR)/drivers/*.o)
# exclude disk from drivers, ports and ports_asm
DRIVERS := $(filter-out $(BUILD_DIR)/drivers/disk.o, $(DRIVERS))
DRIVERS := $(filter-out $(BUILD_DIR)/drivers/disk_asm.o, $(DRIVERS))


.PHONY: all clean always

all: run

run: $(BUILD_DIR)/kernel.bin 

$(BUILD_DIR)/kernel.bin: $(C_OBJECTS) $(ASM_OBJECTS) $(DRIVERS) 
	g++ -m32 -ffreestanding -fno-pie -T linker.ld -nostdlib -Wl,--oformat=binary -Wl,-Map=$(BUILD_DIR)/kernel.map -o $@ $^ -lgcc -std=c++2a 

$(BUILD_DIR)/%.o: %.cpp ${HEADERS} always
	g++ -c -m32 -ffreestanding -nostdlib -fno-pie -fno-exceptions $< -o $@ -std=c++2a -lstdc++


$(BUILD_DIR)/%.o: %.asm
	nasm -f elf -o $@ $<

$(BUILD_DIR)/Interrupts:
	mkdir -p $@

always:
	mkdir -p $(BUILD_DIR)/i686
	mkdir -p $(BUILD_DIR)/hal
	mkdir -p $(BUILD_DIR)/util
	

clean:
	@rm -rf $(BUILD_DIR)/kernel.bin
	@rm -rf $(BUILD_DIR)/kernel.map
	@rm -rf $(BUILD_DIR)/Interrupts/*.o
	@rm -rf $(BUILD_DIR)/*.o