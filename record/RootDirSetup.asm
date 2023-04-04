;
; Read and setup the Root directory
;

read_root_dir:

    ; read something from floppy disk
    ; BIOS should set DL to drive number
    mov [drive_number], dl

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

        ; search for sector.bin
        xor bx, bx
        mov di, buffer

    .check_for_sector:
        mov si, file_sector_bin
        mov cx, 11                          ; compare up to 11 characters
        push di
        repe cmpsb
        pop di
        je .sector_found

        add di, 32
        inc bx
        cmp bx, [dir_entries_count]
        jl .check_for_sector

        ; sector not found
        jmp sector_not_found

    .sector_found:

        ; di should have the address to the entry
        mov ax, [di + 26]                   ; first logical cluster field (offset 26)
        mov [sector_cluster], ax

        ; load FAT from disk into memory
        mov ax, [reserved_sectors]
        mov bx, buffer
        mov cl, [sectors_per_fat]
        mov dl, [drive_number]
        call disk_read

        ; read sector and process FAT chain
        mov bx, sector_SEGMENT
        mov es, bx
        mov bx, sector_OFFSET

    .load_sector_loop:
        
        ; Read next cluster
        mov ax, [sector_cluster]
        
        ; not nice :( hardcoded value
        add ax, 31                          ; first cluster = (sector_cluster - 2) * sectors_per_cluster + start_sector
                                            ; start sector = reserved + fats + root directory size = 1 + 18 + 134 = 33
        mov cl, 1
        mov dl, [drive_number]
        call disk_read

        add bx, [bytes_per_sector]

        ; compute location of next cluster
        mov ax, [sector_cluster]
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

        mov [sector_cluster], ax
        jmp .load_sector_loop

    .last_cluster:
        
        ; jump to our sector
        mov dl, [drive_number]          ; boot device in dl

        mov ax, sector_SEGMENT         ; set segment registers
        mov ds, ax
        mov es, ax

        jmp sector_SEGMENT:sector_OFFSET

        jmp $             ; should never happen
