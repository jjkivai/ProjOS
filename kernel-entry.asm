global _start;
[BITS 32]

_start:
    [extern main]
    call main
    jmp $