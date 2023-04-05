#pragma once
#include <stdint.h>

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
        /* Get location of cursor*/
        int get_cursor_offset();
        /* Set location of cursor */
        void set_cursor_offset(int offset);
        /* Get offset of row and column */
        int get_offset(int row, int col);
        /* Get row from offset */
        int get_row_from_offset(int offset);
        /* Move to a new line*/
        int move_offset_to_new_line(int offset);
        /* Print each character on the line above */
        int scroll_up(int offset);
        /* Convert number to string*/
        void number_to_string(uint32_t number, int radix) {
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
        /* Print error */
        void print_error(const char *error);
};
}