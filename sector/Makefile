BUILD_DIR?= build

C_SOURCES := $(wildcard *.cpp)
C_SOURCES += $(wildcard ../drivers/*.cpp)

ASM_SOURCES := $(wildcard *.asm)

C_OBJECTS := $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(C_SOURCES))
ASM_OBJECTS := $(patsubst %.asm, $(BUILD_DIR)/%.o, $(ASM_SOURCES))

# OBJ_FILES := ${C_SOURCES:.cpp=$(BUILD_DIR)/%.o}

.PHONY: all clean

all: run

run: $(BUILD_DIR)/sector.bin

$(BUILD_DIR)/sector.bin: $(C_OBJECTS) $(ASM_OBJECTS)
	ld -m elf_i386 -Ttext 0x1000  --oformat binary -T linker.ld -o $@ $^ -Map $(BUILD_DIR)/sector.map
	# gcc -T linker.ld -nostdlib -Wl,-Map=$(BUILD_DIR)/sector.map -o $@ $^ -lgcc

$(BUILD_DIR)/%.o: %.cpp ${HEADERS}
	# gcc -ffreestanding -nostdlib -c -o $@ $<
	g++ -c -m32 -ffreestanding -nostdlib -fno-pie  $< -o $@


$(BUILD_DIR)/%.o: %.asm
	nasm -f elf -o $@ $<

# $(BUILD_DIR)/sector.bin: $(BUILD_DIR)/entry.o $(BUILD_DIR)/${OBJ_FILES}
# 	ld -m elf_i386 -Ttext 0x1000  --oformat binary -T linker.ld -o $@ $^

# $(BUILD_DIR)/main.: main.cpp
# 	gcc -c -m32 -ffreestanding -nostdlib -fno-pie  $< -o $@

# $(BUILD_DIR)/%.o: %.cpp #${HEADERS}
# 	gcc -c -m32 -ffreestanding -nostdlib -fno-pie  $< -o $@

# $(BUILD_DIR)/%.o: %.asm
# 	nasm -f elf $< -o $@

clean:
	@rm -rf $(BUILD_DIR)/sector.bin
	@rm -rf $(BUILD_DIR)/sector.map
	@rm -rf $(BUILD_DIR)/*.o