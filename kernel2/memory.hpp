#pragma once
#include <stdint.h>

void *mymemcpy(void *dst, const void *src, uint16_t num);
void *mymemset(void *ptr, int value, uint16_t num);
int mymemcmp(const void *ptr1, const void *ptr2, uint16_t num);
