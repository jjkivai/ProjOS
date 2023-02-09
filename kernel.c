
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

    // int i = 0;
    // while(i < 10) {
    //     set_cursor(cursor);
    //     cursor += 2;
    //     i++;
    // }


    for(int i = 0; message[i] != '\0'; ++i) {
        *video_memory++ = 'E';
        *video_memory++ = 0x07;
        // cursor+=2;
        // set_cursor(cursor);
    }
    
    // *video_memory++ = 'D';
    // *video_memory++ = 0x07;
    // *video_memory++ = 'I';
    // *video_memory++ = 0x07;
    // *video_memory++ = 'G';
    // *video_memory++ = 0x07;
    // *video_memory++ = ' ';
    // *video_memory++ = 0x07;
    // *video_memory++ = 'B';
    // *video_memory++ = 0x07;
    // *video_memory++ = 'I';
    // *video_memory++ = 0x07;
    // *video_memory++ = 'C';
    // *video_memory++ = 0x07;
    // *video_memory++ = 'K';
    // *video_memory++ = 0x07;
    // *video_memory++ = ' ';
    // *video_memory++ = 0x07;
    // *video_memory++ = 'B';
    // *video_memory++ = 0x07;
    // *video_memory++ = 'I';
    // *video_memory++ = 0x07;
    // *video_memory++ = 'O';
    // *video_memory++ = 0x07;
    // *video_memory++ = 'S';
    // *video_memory++ = 0x07;

    // print_string("D");
}
