ENTRY(entry);

SECTIONS {
    . = 0;
    .text : {
        *(.entry)
        *(.text)
    }
    .data : {
        *(.data)
    }
}