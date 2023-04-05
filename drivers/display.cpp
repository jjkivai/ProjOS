/*
Dislplay driver for the VGA text mode

This driver is based on the tutorial from http://www.osdever.net/bkerndev/Docs/printing.htm
as well as the tutorial from http://www.jamesmolloy.co.uk/tutorial_html/3.-The%20Screen.html
and the definition of stdio.h from http://www.cplusplus.com/reference/cstdio/printf/
*/
extern "C" {
    #include "display.hpp"
    #include "ports.hpp"

    #include <stdarg.h>
    #include <stdbool.h>

    #define STATE_NORMAL         0
    #define STATE_LENGTH         1
    #define STATE_LENGTH_SHORT   2
    #define STATE_LENGTH_LONG    3
    #define STATE_SPEC           4

    #define LENGTH_DEFAULT       0
    #define LENGTH_SHORT_SHORT   1
    #define LENGTH_SHORT         2
    #define LENGTH_LONG          3
    #define LENGTH_LONG_LONG     4



    void Display::print_string(const char *string, ...) {
        va_list args;
        va_start(args, string);

        int state = STATE_NORMAL;
        int length = LENGTH_DEFAULT;
        int radix = 10;
        bool negative = false;
        bool zero = false;

            while (*string)
        {
            switch (state)
            {
                case STATE_NORMAL:
                    switch (*string)
                    {
                        case '%':   state = STATE_LENGTH;
                                    break;
                        default:    print_char(*string);
                                    break;
                    }
                    break;

                case STATE_LENGTH:
                    switch (*string)
                    {
                        case 'h':   length = LENGTH_SHORT;
                                    state = STATE_LENGTH_SHORT;
                                    break;
                        case 'l':   length = LENGTH_LONG;
                                    state = STATE_LENGTH_LONG;
                                    break;
                        default:    goto STATE_SPEC_;
                    }
                    break;

                case STATE_LENGTH_SHORT:
                    if (*string == 'h')
                    {
                        length = LENGTH_SHORT_SHORT;
                        state = STATE_SPEC;
                    }
                    else goto STATE_SPEC_;
                    break;

                case STATE_LENGTH_LONG:
                    if (*string == 'l')
                    {
                        length = LENGTH_LONG_LONG;
                        state = STATE_SPEC;
                    }
                    else goto STATE_SPEC_;
                    break;

                case STATE_SPEC:
                STATE_SPEC_:
                    switch (*string)
                    {
                        case 'c':   print_char((char)va_arg(args, int));
                                    break;

                        case 's':   
                                    print_string_s(va_arg(args, const char*));
                                    break;

                        case '%':   print_char('%');
                                    break;

                        case 'd':
                        case 'i':   radix = 10; negative = true; zero = true;
                                    break;

                        case 'u':   radix = 10; negative = false; zero = true;
                                    break;

                        case 'X':
                        case 'x':
                        case 'p':   radix = 16; negative = false; zero = true;
                                    break;

                        case 'o':   radix = 8; negative = false; zero = true;
                                    break;

                        // ignore invalid spec
                        default:    break;
                    }

                    if (zero)
                    {
                        if (negative)
                        {
                            switch (length)
                            {
                            case LENGTH_SHORT_SHORT:
                            case LENGTH_SHORT:
                            case LENGTH_DEFAULT:     print_number(va_arg(args, int), radix);
                                                            break;

                            case LENGTH_LONG:        print_number(va_arg(args, long), radix);
                                                            break;

                            case LENGTH_LONG_LONG:   print_number(va_arg(args, long long), radix);
                                                            break;
                            }
                        }
                        else
                        {
                            switch (length)
                            {
                            case LENGTH_SHORT_SHORT:
                            case LENGTH_SHORT:
                            case LENGTH_DEFAULT:     print_number(va_arg(args, unsigned int), radix);
                                                            break;
                                                            
                            case LENGTH_LONG:        print_number(va_arg(args, unsigned  long), radix);
                                                            break;

                            case LENGTH_LONG_LONG:   print_number(va_arg(args, unsigned  long long), radix);
                                                            break;
                            }
                        }
                    }

                    // reset state
                    state = STATE_NORMAL;
                    length = LENGTH_DEFAULT;
                    radix = 10;
                    negative = false;
                    break;
            }

            string++;
        }

        va_end(args);
    }
    /* Print string only */
    void Display::print_string_s(const char *string) {
        while (*string)
        {
            print_char(*string);
            string++;
        }
    }
    /* print buffer*/
    void Display::print_buffer(const char *string, const void *buffer, int size) {
        const uint8_t *buf = (const uint8_t*)buffer;
        print_string_s(string);
        print_string_s("0x");
        for (int i = 0; i < size; i++)
        {
            print_number((buf[i] >> 4), 16);
            print_number((buf[i]&0xF), 16);
        }
        print_newline();
    }
    /* Print a character */
    void Display::print_char(char character) {
        switch(character) {
            case '\n':
                offset_x = 0;
                offset_y++;
                break;
            case '\t':
                for(int i = 0; i < 4 - (offset_x%4); i++) {
                    print_char(' ');
                }
                break;
            default:
                set_char_at_video_memory(character, offset_x, offset_y);
                offset_x++;
                break;
        }
        if(offset_x >= MAX_COLS) {
            offset_x = 0;
            offset_y++;
        }
        if(offset_y >= MAX_ROWS) {
            scroll_up(1);
            offset_y = MAX_ROWS - 1;
        }
        set_cursor_offset(offset_x, offset_y);
    }
    /* Print a new line*/
    void Display::print_newline(){
        print_string_s("\n");
    }
    /* Clear screen */
    void Display::clear_screen(){
        for(int i = 0; i < MAX_ROWS; i++) {
            for(int j = 0; j < MAX_COLS; j++) {
                set_char_at_video_memory('\0', j, i);
            }
        }
        offset_x = 0;
        offset_y = 0;
        set_cursor_offset(offset_x, offset_y);
    }
    /* Print backspace */
    void Display::backspace() {
        int newCursor = get_cursor_offset() - 2;
        set_char_at_video_memory(' ', newCursor);
        set_cursor_offset(newCursor);
        offset_x--;
    }

}