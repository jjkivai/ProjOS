; load 'dh' sectors from drive 'dl' into ES:BX
disk_load:
    ; Push registers which will be used
    push ax
    push bx
    push cx
    push dx
    push di

    push cx ; save the number of sectors to read

    ; Set up the disk parameters
    push ax
    push dx
    xor dx, dx ; clear dx
    div word [sectors_per_track] ; dx = track number, ax = sector number
    inc dx ; track number is 1-based
    mov cx, dx ; cx = sector number

    xor dx, dx ; clear dx
    div word [number_of_heads] ; dx = head number, ax = cylinder number
    mov dh, dl ; dh = head number
    mov ch, al ; ch = cylinder number
    shl ah, 6
    or cl, ah ; cl = sector number

    pop ax
    mov dl, al ; dl = drive number
    pop ax
    pop ax

    ; Read the sectors
    mov ah, 0x02

    mov di, 3

    .disk_read:
        pusha
        stc
        int 0x13
        jnc .disk_read_ok
        popa
        call disk_reset

        dec di
        test di, di
        jnz .disk_read
    .fail:
        jmp disk_error
    .disk_read_ok:
        popa

        pop di
        pop dx
        pop cx
        pop bx
        pop ax                             ; restore registers modified
        ret

disk_reset:
    pusha
    mov ah, 0
    stc
    int 0x13
    jc disk_error
    popa
    ret

disk_error:
    mov si, DISK_ERROR
    call print_string
    call print_newline
    mov dh, ah ; ah = error code, dl = disk drive that dropped the error
    jmp disk_loop


disk_loop:
    jmp $

DISK_ERROR: db "Disk read error", 0
