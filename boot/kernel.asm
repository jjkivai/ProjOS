;global _start
;[BITS 32]

; start kernel
;_start:
    ; [extern main]
    ; call main
 ;   jmp $

[ORG 0x00]
[BITS 16]


%define ENDL 0x0D, 0x0A


start:
    ; print hello world message
    mov si, msg_hello
    call print_string

    .halt:
        cli
        hlt


msg_hello: db 'Hello world from KERNEL!', ENDL, 0