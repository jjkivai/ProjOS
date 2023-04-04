BUILD_DIR?= build

C_SOURCES := $(wildcard */*.cpp *.cpp)
HEADERS := $(wildcard */*.h *.h)

OBJ_FILES := ${C_SOURCES:.cpp=.o}

.PHONY: all clean

all: run

run: $(BUILD_DIR)/sector.bin

$(BUILD_DIR)/sector.bin: entry.o ${OBJ_FILES}
	ld -m elf_i386 -Ttext 0x1000  --oformat binary -T linker.ld -o $@ $^


$(BUILD_DIR)/%.bin: %.asm
	nasm -f bin $< -o $@

$(BUILD_DIR)/%.o: %.cpp ${HEADERS}
	gcc -m32 -ffreestanding -fno-pie -c $< -o $@

$(BUILD_DIR)/%.o: %.asm
	nasm -f elf $< -o $@

clean:
	@rm -rf $(BUILD_DIR)/sector.bin