BUILD_DIR?= build

C_SOURCES := $(wildcard *.cpp)
C_SOURCES += $(wildcard ../drivers/*.cpp)

ASM_SOURCES := $(wildcard *.asm)
ASM_SOURCES += $(wildcard ../drivers/*.asm)

C_OBJECTS := $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(C_SOURCES))
ASM_OBJECTS := $(patsubst %.asm, $(BUILD_DIR)/%.o, $(ASM_SOURCES))

HEADERS := $(wildcard *.hpp)
HEADERS += $(wildcard ../drivers/*.hpp)
# OBJ_FILES := ${C_SOURCES:.cpp=$(BUILD_DIR)/%.o}

.PHONY: all clean

all: run

run: $(BUILD_DIR)/kernel.bin

$(BUILD_DIR)/kernel.bin: $(C_OBJECTS) $(ASM_OBJECTS)
	#ld -m elf_i386 -Ttext 0x1000  --oformat binary -T linker.ld -o $@ $^ -Map $(BUILD_DIR)/kernel.map 
	g++ -m32 -ffreestanding -fno-pie -T linker.ld -nostdlib -Wl,-Ttext,0x1000 -Wl,--oformat=binary -Wl,-Map=$(BUILD_DIR)/kernel.map -o $@ $^ -lgcc -std=c++2a 

$(BUILD_DIR)/%.o: %.cpp ${HEADERS}
	# gcc -ffreestanding -nostdlib -c -o $@ $<
	g++ -c -m32 -ffreestanding -nostdlib -fno-pie -fno-exceptions $< -o $@ -std=c++2a -lstdc++


$(BUILD_DIR)/%.o: %.asm
	nasm -f elf -o $@ $<

# $(BUILD_DIR)/kernel.bin: $(BUILD_DIR)/entry.o $(BUILD_DIR)/${OBJ_FILES}
# 	ld -m elf_i386 -Ttext 0x1000  --oformat binary -T linker.ld -o $@ $^

# $(BUILD_DIR)/main.: main.cpp
# 	gcc -c -m32 -ffreestanding -nostdlib -fno-pie  $< -o $@

# $(BUILD_DIR)/%.o: %.cpp #${HEADERS}
# 	gcc -c -m32 -ffreestanding -nostdlib -fno-pie  $< -o $@

# $(BUILD_DIR)/%.o: %.asm
# 	nasm -f elf $< -o $@

clean:
	@rm -rf $(BUILD_DIR)/kernel.bin
	@rm -rf $(BUILD_DIR)/kernel.map
	@rm -rf $(BUILD_DIR)/*.o