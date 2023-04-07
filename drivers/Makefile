BUILD_DIR?= build

C_SOURCES := $(wildcard *.cpp)
ASM_SOURCES := $(wildcard *.asm)

C_OBJECTS := $(patsubst %.cpp, $(BUILD_DIR)/drivers/%.o, $(C_SOURCES))
ASM_OBJECTS := $(patsubst %.asm, $(BUILD_DIR)/drivers/%.o, $(ASM_SOURCES))

HEADERS := $(wildcard *.hpp)

.PHONY: all clean

all: run

run: $(BUILD_DIR)/drivers.o

$(BUILD_DIR)/drivers.o: $(C_OBJECTS) $(ASM_OBJECTS)
	g++ -m32 -ffreestanding -fno-pie -nostdlib -Wl,--oformat=binary -Wl,-Map=$(BUILD_DIR)/drivers.map -o $@ $^ -lgcc -std=c++2a 

$(BUILD_DIR)/drivers/%.o: %.cpp ${HEADERS}
	g++ -c -m32 -ffreestanding -nostdlib -fno-pie -fno-exceptions $< -o $@ -std=c++2a -lstdc++ 

$(BUILD_DIR)/drivers/%.o: %.asm
	nasm -f elf -o $@ $<

clean:
	@rm -rf $(BUILD_DIR)/drivers/drivers.o
	@rm -rf $(BUILD_DIR)/drivers/drivers.map
	@rm -rf $(BUILD_DIR)/*.o
