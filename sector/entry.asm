; Technically stage 2

[BITS 16]

section .entry

extern __bss_start
extern __end


extern start
global entry

entry:
    cli
    ; save boot drive
    mov [g_BootDrive], dl
    mov [g_BootPartitionOffset], si
    mov [g_BootPartitionSegment], di

    ; setup stack
    mov ax, ds
    mov ss, ax
    mov sp, 0xFFF0
    mov bp, sp
    
    ;Switch to protected mode
    call EnableA20 ; enable A20 line
    call LoadGDT ; load GDT

    ; 4 - set protection enable flag in CR0
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    jmp dword 0x08:.pmode ; far jump to protected mode

    .pmode:
        ; we are now in protected mode!
        [bits 32]
        
        ; 6 - setup segment registers
        mov ax, 0x10
        mov ds, ax
        mov ss, ax
    
        ; clear bss (uninitialized data)
        mov edi, __bss_start
        mov ecx, __end
        sub ecx, edi
        mov al, 0
        cld
        rep stosb

        ; expect boot drive in dl, send it as argument to cstart function
        xor edx, edx
        mov dl, [g_BootDrive]
        push edx

        call start

        cli
        hlt



%include "EnableA20.asm" ; Contains GDT

%include "GDT.asm"

LoadGDT:
    [bits 16]
    lgdt [g_GDTDesc]
    ret

g_BootDrive: db 0
g_BootPartitionSegment: dw 0
g_BootPartitionOffset: dw 0

