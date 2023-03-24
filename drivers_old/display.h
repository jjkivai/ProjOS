#ifndef DISPLAY_H
#define DISPLAY_H
#pragma once
#include <stdint.h>
#define VGA_CTRL_REGISTER 0x3D4
#define VGA_DATA_REGISTER 0x3D5
#define VGA_OFFSET_LOW 0x0F
#define VGA_OFFSET_HIGH 0x0E
#define MAX_ROWS 25
#define MAX_COLS 80
#define WHITE_ON_BLACK 0x07
#define VIDEO_ADDRESS 0xB8000

unsigned char port_byte_in(unsigned short port)
{
    unsigned char result;
    __asm__("in %%dx, %%al"
            : "=a"(result)
            : "d"(port));
    return result;
}

void port_byte_out(unsigned short port, unsigned char data)
{
    __asm__("out %%al, %%dx"
            :
            : "a"(data), "d"(port));
}
void set_cursor(int offset)
{
    offset /= 2;
    port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
    port_byte_out(VGA_DATA_REGISTER, (unsigned char)(offset >> 8));
    port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
    port_byte_out(VGA_DATA_REGISTER, (unsigned char)(offset & 0xff));
}

int get_cursor()
{
    port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
    int offset = port_byte_in(VGA_DATA_REGISTER) << 8;
    port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
    offset += port_byte_in(VGA_DATA_REGISTER);
    return offset * 2;
}
void set_char_at_video_memory(char character, int offset)
{
    unsigned char *video_memory = (unsigned char *)VIDEO_ADDRESS;
    video_memory[offset] = character;
    video_memory[offset + 1] = WHITE_ON_BLACK;
}
int get_row_from_offset(int offset)
{
    return offset / (2 * MAX_COLS);
}

int get_offset(int col, int row)
{
    return 2 * (row * MAX_COLS + col);
}
int move_offset_to_new_line(int offset)
{
    return get_offset(0, get_row_from_offset(offset) + 1);
}
void memory_copy(char *source, char *dest, int nbytes)
{
    int i;
    for (i = 0; i < nbytes; i++)
    {
        *(dest + i) = *(source + i);
    }
}
int scroll_ln(int offset)
{
    memory_copy(
        (char *)(get_offset(0, 1) + VIDEO_ADDRESS),
        (char *)(get_offset(0, 0) + VIDEO_ADDRESS),
        MAX_COLS * (MAX_ROWS - 1) * 2);

    for (int col = 0; col < MAX_COLS; col++)
    {
        set_char_at_video_memory(' ', get_offset(col, MAX_ROWS - 1));
    }

    return offset - 2 * MAX_COLS;
}
void print_string(const char *string)
{
    int offset = get_cursor();
    int i = 0;
    while (string[i] != 0)
    {
        char v = string[i];
        if (offset >= MAX_ROWS * MAX_COLS * 2)
        {
            offset = scroll_ln(offset);
        }
        if (string[i] == '\n')
        {
            offset = move_offset_to_new_line(offset);
        }
        else
        {
            set_char_at_video_memory(v, offset);
            offset += 2;
        }
        i++;
    }
    set_cursor(offset);
}
void clear_screen()
{
    for (int i = 0; i < MAX_COLS * MAX_ROWS; ++i)
    {
        set_char_at_video_memory(' ', i * 2);
    }
    set_cursor(get_offset(0, 0));
}
void print_backspace()
{
    int newCursor = get_cursor() - 2;
    set_char_at_video_memory(' ', newCursor);
    set_cursor(newCursor);
}
void print_nl()
{
    int newOffset = move_offset_to_new_line(get_cursor());
    if (newOffset >= MAX_ROWS * MAX_COLS * 2)
    {
        newOffset = scroll_ln(newOffset);
    }
    set_cursor(newOffset);
}

#endif