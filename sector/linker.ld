ENTRY(entry)
OUTPUT_FORMAT("binary")
phys = 0x00000500;

SECTIONS
{
    . = phys;

    .entry              : { __entry_start = .;      *(.entry)   }
    .text ALIGN(4k)              : { __text_start = .;       *(.text)    }
    .data ALIGN(4k)                : { __data_start = .;       *(.data)    }
    .rodata  ALIGN(4k)             : { __rodata_start = .;     *(.rodata)  }
    .bss  ALIGN(4k)                : { __bss_start = .;        *(.bss)     }
    
    __end = .;
}