#pragma once
#include <stdint.h>

extern "C" void i686_outb(uint16_t port, uint8_t value);
extern "C" uint8_t i686_inb(uint16_t port);

extern "C" void i686_Panic();