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
                            case LENGTH_DEFAULT:     number_to_string(va_arg(args, int), radix);
                                                            break;

                            case LENGTH_LONG:        number_to_string(va_arg(args, long), radix);
                                                            break;

                            case LENGTH_LONG_LONG:   number_to_string(va_arg(args, long long), radix);
                                                            break;
                            }
                        }
                        else
                        {
                            switch (length)
                            {
                            case LENGTH_SHORT_SHORT:
                            case LENGTH_SHORT:
                            case LENGTH_DEFAULT:     number_to_string(va_arg(args, unsigned int), radix);
                                                            break;
                                                            
                            case LENGTH_LONG:        number_to_string(va_arg(args, unsigned  long), radix);
                                                            break;

                            case LENGTH_LONG_LONG:   number_to_string(va_arg(args, unsigned  long long), radix);
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

    }
    /* print buffer*/
    void Display::print_buffer(const char *string, const void *buffer, int size) {
        const uint8_t *buf = (const uint8_t*)buffer;
        print_string_s(string);
        for (int i = 0; i < size; i++)
        {
            print_string_s("0x");
            number_to_string(buf[i], 16);
            print_string_s(" ");
        }
        print_newline();
    }
    /* Print a character */
    void Display::print_char(char character) {

    }
    /* Print a new line*/
    void Display::print_newline(){
        print_string_s("\n");
    }
    /* Clear screen */
    void Display::clear_screen(){

    }
    /* Print backspace */
    void Display::backspace() {

    }
    /* Print error */
    void Display::print_error(const char *error) {

    }
}