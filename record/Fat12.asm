;
; FAT12 header
; 
oem_name:               db "PROJOSv1"           ; 8 bytes
bytes_per_sector:       dw 512
sectors_per_cluster:    db 1
reserved_sectors:       dw 1
fat_count:              db 2
dir_entries_count:      dw 0xE0
total_sectors:          dw 2880                 ; 2880 * 512 = 1.44MB
media_descriptor_type:  db 0xF0                 ; F0 = 3.5" floppy disk
sectors_per_fat:        dw 9                    ; 9 sectors/fat
sectors_per_track:      dw 18
number_of_heads:        dw 2
hidden_sectors:         dd 0
large_sectors:          dd 0

; extended boot record
drive_number:           db 0                    ; 0x00 floppy, 0x80 hdd, useless
                        db 0                    ; reserved
signature:              db 0x29
volume_id:              db 0x12, 0x34, 0x56, 0x78   ; serial number, value doesn't matter
volume_label:           db "PROJECT OS"      ; 11 bytes, padded with spaces
system_id:              db 'FAT12   '           ; 8 bytes
