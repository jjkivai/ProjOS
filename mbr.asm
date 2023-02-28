[BITS 16]
[ORG 0x7C00]

jmp start

; Print a string to the screen
print_string:
    pusha
    mov ah, 0x0E
    .next_char:
        lodsb
        cmp al, 0
        je .done
        int 0x10
        jmp .next_char
    .done:
        popa
        ret

; Entry point of the BIOS
start:
    mov si, message
    call print_string
    ; where kernel will go
    KERNEL_OFFSET equ 0x1000
    ; BIOS sets boot drive
    mov [BOOT_DRIVE], dl
    ; setup stack
    mov bp, 0x9000
    mov sp, bp

    call load_kernel
    call switch_to_32bit

    jmp $

    %include "Disk.asm"
    %include "setupGDT.asm"
    %include "switch_to_32bit.asm"

    ;Segment reg
    mov ax, 0x07C0
    mov ds, ax
    mov ss, ax
    mov sp, 0x7C00
    ;Load disk
    mov ah, 0x02
    mov al, 0x01
    mov ch, 0x00
    mov cl, 0x02
    mov dh, 0x00
    mov dl, 0x80
    int 0x13
    ;IDFK
    jmp 0x0000:0x7C00

message db 'This is the Project Boot Loader', 0


BOOT_DRIVE db 0
[BITS 16]
load_kernel:
    mov bx, KERNEL_OFFSET;
    mov dh, 2
    mov dl, [BOOT_DRIVE]
    call disk_load
    ret

[BITS 32]
BEGIN_32BIT:
    call KERNEL_OFFSET
    jmp $;

; BOOT_DRIVE VAR



; Fill the rest of the sector with zeros
times 510 - ($ - $$) db 0
dw 0xAA55
