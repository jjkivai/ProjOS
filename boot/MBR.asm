; Master Boot Record
; This is where the kernel will go to when OS starts
[ORG 0x7C00]
[BITS 16]


%define ENDL 0x0D, 0x0A


;
jmp short start
nop

;
; FAT12 header
; 
%include "boot/Fat12.asm"
;
; Code goes here

start:
    ; setup data segments
    mov ax, 0           ; can't set ds/es directly
    mov ds, ax
    mov es, ax
    
    ; setup stack
    mov ss, ax
    mov sp, 0x7C00              ; stack grows downwards from where we are loaded in memory

    ; some BIOSes might start us at 07C0:0000 instead of 0000:7C00, make sure we are in the
    ; expected location
    push es
    ; Read root directory and Load kernel from directory
    push word read_root_dir
    retf

%include "boot/RootDirSetup.asm"


;
; Error handlers
;

kernel_not_found:
    mov si, msg_kernel_not_found
    call print_string
    jmp $

;
; Prints a string to the screen
; Params:
;   - ds:si points to string
; 
print_string:
    pusha ;store context
    mov ah, 0x0E ; Begin print
    .next_char:
        ; Load data in si
        lodsb ; mov al, [si]
        cmp al, 0 ; check if string end
        je .print_done ; return context if done
        int 0x10 ; call to print char
        jmp .next_char ; loop
    .print_done:
        popa ;return context
        ret ; return to print caller

;
; Disk routines
;

%include "boot/DiskLoader.asm"



msg_kernel_not_found:   db "KERNEL.BIN file not found!", ENDL, 0
file_kernel_bin:        db "KERNEL  BIN"
kernel_cluster:         dw 0

; Location of kernerl from start of program
; Memory address of kernel
KERNEL_OFFSET equ 0
KERNEL_SEGMENT equ 0x1000


times 510-($-$$) db 0
dw 0AA55h

buffer: