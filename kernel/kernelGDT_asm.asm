[bits 32]

; gdt_start and gdt_end labels are used to compute size

; null segment descriptor

gdt_start:
    dd 0x0
    dd 0x0

; code segment descriptor
gdt_code:
    dw 0xffff    ; segment length, bits 0-15
    dw 0x0       ; segment base, bits 0-15
    db 0x0       ; segment base, bits 16-23
    db 10011010b ; flags (8 bits)
    db 11001111b ; flags (4 bits) + segment length, bits 16-19
    db 0x0       ; segment base, bits 24-31

; data segment descriptor
gdt_data:
    dw 0xffff    ; segment length, bits 0-15
    dw 0x0       ; segment base, bits 0-15
    db 0x0       ; segment base, bits 16-23
    db 10010010b ; flags (8 bits)
    db 11001111b ; flags (4 bits) + segment length, bits 16-19
    db 0x0       ; segment base, bits 24-31

; leave empty
gdt_end:

; GDT descriptor
gdt_descriptor:
    dw gdt_end - gdt_start - 1 ; size (16 bit), always one less of its true size
    dd gdt_start ; address (32 bit)



; void GDT_Load();
global GDT_Load
GDT_Load:
    push ebp
    mov ebp, esp

    lgdt [gdt_descriptor]
    mov eax, 0x08
    ;mov eax, [ebp + 12]
    push eax
    push .reload_segments
    retf

    .reload_segments:
        mov ax, 0x10
        ;mov ax, [ebp + 16]
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax
        mov ss, ax

        mov esp, ebp
        pop ebp
        ret


; define some constants for later use
CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start