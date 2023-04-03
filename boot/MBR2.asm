; Master Boot Record
; This is where the kernel will go to when OS starts
[ORG 0x7C00]
[BITS 16]


%define ENDL 0x0D, 0x0A

; Location of kernerl from start of program
; Memory address of kernel
KERNEL_OFFSET equ 0x1000
KERNEL_SEGMENT equ 0


jmp short start
nop


;
; Code goes here
;

start:
    mov si, welcome_message ; Load welcome message
    call print_string ; Print welcome message
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
    push word read_root_dir
    retf




%include "boot/DiskLoader.asm"
%include "boot/FatSetup.asm"
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



welcome_message db "Master Boot Record",ENDL, 0

msg_kernel_not_found:   db 'KERNEL.BIN file not found!', ENDL, 0


file_kernel_bin:        db 'KERNEL  BIN'
kernel_cluster:         dw 0



times 510-($-$$) db 0
dw 0AA55h

buffer: