
#include "drivers/display.h"

void main() {
    clear_screen();
    int cursor = get_cursor();

    char* video_memory = (char*) 0xB8000;
    const char* message = "Hello, world";
    *video_memory++ = 'L';
    *video_memory++ = 0x07;
    cursor+=2;
    set_cursor(cursor);
    *video_memory++ = 'O';
    *video_memory++ = 0x07;
    cursor+=2;
    set_cursor(cursor);
    *video_memory++ = 'V';
    *video_memory++ = 0x07;
    cursor+=2;
    set_cursor(cursor);
    *video_memory++ = 'E';
    *video_memory++ = 0x07;
    cursor+=2;
    set_cursor(cursor);
    cursor+=2;
    set_cursor(cursor);
    cursor+=2;
    set_cursor(cursor);
    cursor+=2;
    set_cursor(cursor);
    cursor+=2;
    set_cursor(cursor);
    cursor+=2;
    set_cursor(cursor);


    // for(int i = 0; message[i] != '\0'; ++i) {
    //     *video_memory++ = 'E';
    //     *video_memory++ = 0x07;
    //     // cursor+=2;
    //     // set_cursor(cursor);
    // }
    


    print_string("DAVID");
}
