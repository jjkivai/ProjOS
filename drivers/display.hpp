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
    public:
        /* Print string to screen */
        void print(const char *character);
        /* Print number to screen */
        void print(int number);
        /* Print a new line*/
        void print_newline();
        /* Clear screen */
        void clear();
        /* Print backspace */
        void backspace();
        /* Print error */
        void print_error(const char *error);
};