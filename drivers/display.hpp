#pragma once
#include <stdint.h>
#include "ports.hpp"

#define VIDEO_ADDRESS 0xb8000 // VGA text mode buffer
#define MAX_ROWS 25 // Maximum size of printable rows
#define MAX_COLS 80 // Maximum size of printable columns
#define WHITE_ON_BLACK 0x0f // Default color scheme
#define RED_ON_BLACK 0x04 // Error color scheme

// Screen device I/O ports
#define VGA_SCREEN_CTRL 0x3D4 // VGA screen control port
#define VGA_SCREEN_DATA 0x3D5 // VGA screen data port

// Display class
extern "C" {
class Display {
    private:
        /* Screen offsets*/
        static int offset_x;
        static int offset_y;
        /* Get location of cursor*/
        int get_cursor_offset() {
            port_byte_out(VGA_SCREEN_CTRL, 14); // Tell VGA screen we are sending high byte of cursor offset
            int offset = port_byte_in(VGA_SCREEN_DATA) << 8; // Get high byte of cursor offset
            port_byte_out(VGA_SCREEN_CTRL, 15); // Tell VGA screen we are sending low byte of cursor offset
            offset += port_byte_in(VGA_SCREEN_DATA); // Get low byte of cursor offset
            return offset * 2; // Multiply by two since the cursor offset is calculated in bytes
        }
        /* Set location of cursor */
        void set_cursor_offset(int offset) {
            offset /= 2; // Divide by two since the cursor offset is calculated in bytes
            port_byte_out(VGA_SCREEN_CTRL, 14); // Tell VGA screen we are setting high byte of cursor offset
            port_byte_out(VGA_SCREEN_DATA, (uint8_t)(offset >> 8)); // Send high byte
            port_byte_out(VGA_SCREEN_CTRL, 15); // Tell VGA screen we are setting low byte of cursor offset
            port_byte_out(VGA_SCREEN_DATA, (uint8_t)(offset & 0xff)); // Send low byte
        }
        /* Set location of cursor */
        void set_cursor_offset(int x, int y) {
            int offset = 2*(y * MAX_COLS + x);
            set_cursor_offset(offset);
            
            //port_byte_out(VGA_SCREEN_CTRL, 0x0F);
            //port_byte_out(VGA_SCREEN_DATA, (uint8_t)(offset & 0xFF));
            //port_byte_out(VGA_SCREEN_CTRL, 0x0E);
            //port_byte_out(VGA_SCREEN_DATA, (uint8_t)((offset >> 8) & 0xFF));
            
        }
        /* Print character to screen */
        void set_char_at_video_memory(char character, int offset) {
            uint8_t *video_memory = (uint8_t*)VIDEO_ADDRESS;
            video_memory[offset] = character;
        }
        /* Print character to screen */
        void set_char_at_video_memory(char character, int x, int y) {
            int pos = 2 * (y * MAX_COLS + x);
            set_char_at_video_memory(character, pos);
        }
        /* Get offset of row and column */
        int get_offset(int row, int col) {
            return 2 * (row * MAX_COLS + col);
        }
        /* Get row from offset */
        int get_row_from_offset(int offset) {
            return offset / (2 * MAX_COLS);
        }
        /* Move to a new line*/
        int move_offset_to_new_line(int offset) {
            return get_offset(get_row_from_offset(offset) + 1, 0);
        }
        /* Copy each line*/
        void memory_copy(char *source, char *dest, int nbytes) {
            int i;
            for (i = 0; i < nbytes; i++)
            {
                *(dest + i) = *(source + i);
            }
        }
        /* Print each character on the line above */
        void scroll_up(int lines) {
            for(int i = 0; i < lines; i++) {

                memory_copy(
                    (char *)(get_offset(1, 0) + VIDEO_ADDRESS),
                    (char *)(get_offset(0, 0) + VIDEO_ADDRESS),
                    MAX_COLS * (MAX_ROWS - 1) * 2);

                for (int col = 0; col < MAX_COLS; col++)
                {
                    set_char_at_video_memory(' ', get_offset( MAX_ROWS - 1, col));
                }
            }
            offset_y -= lines;
        }
        /* Convert number to string*/
        void print_number(uint32_t number, int radix) {
            char buffer[32];
            // Wok on printing hex numbers
            int pos=0, sign;
            if ((sign = number) < 0) number = -number;
            do {
                buffer[pos++] = number % radix + '0';
            } while((number /= radix) > 0);

            // Print in reverse order
            if (sign < 0) buffer[pos++] = '-';
            while(pos > 0) {
                print_char(buffer[--pos]);
            }
        }
    public:
        /* Print a string*/
        void print_string(const char *string, ...);
        /* Print string only */
        void print_string_s(const char *string);
        /* print buffer*/
        void print_buffer(const char *string, const void *buffer, int size);
        /* Print a character */
        void print_char(char character);
        /* Print a new line*/
        void print_newline();
        /* Clear screen */
        void clear_screen();
        /* Print backspace */
        void backspace();


    };
}