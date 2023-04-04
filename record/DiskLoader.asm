;
; Disk routines
;
;
; Reads sectors from a disk
; Parameters:
;   - ax: LBA address
;   - cl: number of sectors to read (up to 128)
;   - dl: drive number
;   - es:bx: memory address where to store read data
;
disk_read:

    push ax                             ; save registers we will modify
    push bx
    push cx
    push dx
    push di

    push cx                             ; temporarily save CL (number of sectors to read)
    
        push ax
    push dx

    xor dx, dx                          ; dx = 0
    div word [sectors_per_track]    ; ax = LBA / SectorsPerTrack
                                        ; dx = LBA % SectorsPerTrack

    inc dx                              ; dx = (LBA % SectorsPerTrack + 1) = sector
    mov cx, dx                          ; cx = sector

    xor dx, dx                          ; dx = 0
    div word [number_of_heads]                ; ax = (LBA / SectorsPerTrack) / Heads = cylinder
                                        ; dx = (LBA / SectorsPerTrack) % Heads = head
    mov dh, dl                          ; dh = head
    mov ch, al                          ; ch = cylinder (lower 8 bits)
    shl ah, 6
    or cl, ah                           ; put upper 2 bits of cylinder in CL

    pop ax
    mov dl, al                          ; restore DL
    pop ax

    pop ax                              ; AL = number of sectors to read
    
    mov ah, 0x02                       ; AH = 0x02 = read sectors

    pusha                               ; save all registers, we don't know what bios modifies
    stc                                 ; set carry flag, some BIOS'es don't set it
    int 0x13                            ; carry flag cleared = success
    jc disk_error                           ; jump if carry not set

    mov si, DISK_SUCCESS
    call print_string
    popa                               ; restore registers

    pop di                             ; restore registers we modified
    pop dx
    pop cx
    pop bx
    pop ax                             ; restore registers modified
    ret


disk_error:
    mov si, DISK_ERROR
    call print_string
    jmp $

DISK_ERROR:        db "Disk read error", ENDL, 0
DISK_SUCCESS:       db "Disk read ok", ENDL, 0