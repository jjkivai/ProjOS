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
    push word .read_root_dir
    retf

    .read_root_dir:

        ; read something from floppy disk
        ; BIOS should set DL to drive number
        mov [drive_number], dl

        ; show loading message
        mov si, msg_loading
        call print_string

        ; read drive parameters (sectors per track and head count),
        ; instead of relying on data on formatted disk
        push es
        mov ah, 0x08
        int 0x13
        jc disk_error
        pop es

        and cl, 0x3F                        ; remove top 2 bits
        xor ch, ch
        mov [sectors_per_track], cx     ; sector count

        inc dh
        mov [number_of_heads], dh                 ; head count

        ; compute LBA of root directory = reserved + fats * sectors_per_fat
        ; note: this section can be hardcoded
        mov ax, [sectors_per_fat]
        mov bl, [fat_count]
        xor bh, bh
        mul bx                              ; ax = (fats * sectors_per_fat)
        add ax, [reserved_sectors]      ; ax = LBA of root directory
        push ax

        ; compute size of root directory = (32 * number_of_entries) / bytes_per_sector
        mov ax, [dir_entries_count]
        shl ax, 5                           ; ax *= 32
        xor dx, dx                          ; dx = 0
        div word [bytes_per_sector]     ; number of sectors we need to read

        test dx, dx                         ; if dx != 0, add 1
        jz .root_dir_correct_size
        inc ax                              ; division remainder != 0, add 1
                                        ; this means we have a sector only partially filled with entries
    .root_dir_correct_size:

        ; read root directory
        mov cl, al                          ; cl = number of sectors to read = size of root directory
        pop ax                              ; ax = LBA of root directory
        mov dl, [drive_number]          ; dl = drive number (we saved it previously)
        mov bx, buffer                      ; es:bx = buffer
        call disk_read

        ; search for kernel.bin
        xor bx, bx
        mov di, buffer

    .check_for_kernel:
        mov si, file_kernel_bin
        mov cx, 11                          ; compare up to 11 characters
        push di
        repe cmpsb
        pop di
        je .kernel_found

        add di, 32
        inc bx
        cmp bx, [dir_entries_count]
        jl .check_for_kernel

        ; kernel not found
        jmp kernel_not_found

    .kernel_found:

        ; di should have the address to the entry
        mov ax, [di + 26]                   ; first logical cluster field (offset 26)
        mov [kernel_cluster], ax

        ; load FAT from disk into memory
        mov ax, [reserved_sectors]
        mov bx, buffer
        mov cl, [sectors_per_fat]
        mov dl, [drive_number]
        call disk_read

        ; read kernel and process FAT chain
        mov bx, KERNEL_SEGMENT
        mov es, bx
        mov bx, KERNEL_OFFSET

    .load_kernel_loop:
        
        ; Read next cluster
        mov ax, [kernel_cluster]
        
        ; not nice :( hardcoded value
        add ax, 31                          ; first cluster = (kernel_cluster - 2) * sectors_per_cluster + start_sector
                                            ; start sector = reserved + fats + root directory size = 1 + 18 + 134 = 33
        mov cl, 1
        mov dl, [drive_number]
        call disk_read

        add bx, [bytes_per_sector]

        ; compute location of next cluster
        mov ax, [kernel_cluster]
        mov cx, 3
        mul cx
        mov cx, 2
        div cx                              ; ax = index of entry in FAT, dx = cluster mod 2

        mov si, buffer
        add si, ax
        mov ax, [ds:si]                     ; read entry from FAT table at index ax

        or dx, dx
        jz .even
        shr ax, 4
        jmp .read_next_cluster

    .even:
        and ax, 0x0FFF

    .read_next_cluster:
        cmp ax, 0x0FF8                      ; end of chain
        jae .last_cluster

        mov [kernel_cluster], ax
        jmp .load_kernel_loop

    .last_cluster:
        
        ; jump to our kernel
        mov dl, [drive_number]          ; boot device in dl

        mov ax, KERNEL_SEGMENT         ; set segment registers
        mov ds, ax
        mov es, ax

        jmp KERNEL_SEGMENT:KERNEL_OFFSET

        jmp $             ; should never happen

        cli                                 ; disable interrupts, this way CPU can't get out of "halt" state
        hlt

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
msg_loading:            db 'Loading.', ENDL, 0

msg_kernel_not_found:   db 'KERNEL.BIN file not found!', ENDL, 0


file_kernel_bin:        db 'KERNEL  BIN'
kernel_cluster:         dw 0



times 510-($-$$) db 0
dw 0AA55h

buffer: