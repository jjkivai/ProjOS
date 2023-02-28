#![allow(
    dead_code,
    mutable_transmutes,
    non_camel_case_types,
    non_snake_case,
    non_upper_case_globals,
    unused_assignments,
    unused_mut
)]
#![register_tool(c2rust)]
#![feature(asm, register_tool)]
use std::arch::asm;
#[no_mangle]
pub unsafe extern "C" fn port_byte_in(mut port: libc::c_ushort) -> libc::c_uchar {
    let mut result: libc::c_uchar = 0;
    asm!(
        "in %dx, %al", lateout("ax") result, inlateout("dx") port => _,
        options(preserves_flags, pure, readonly, att_syntax)
    );
    return result;
}
#[no_mangle]
pub unsafe extern "C" fn port_byte_out(mut port: libc::c_ushort, mut data: libc::c_uchar) {
    asm!(
        "out %al, %dx", inlateout("dx") port => _, inlateout("ax") data => _,
        options(preserves_flags, att_syntax)
    );
}
#[no_mangle]
pub unsafe extern "C" fn set_cursor(mut offset: libc::c_int) {
    offset /= 2 as libc::c_int;
    port_byte_out(
        0x3d4 as libc::c_int as libc::c_ushort,
        0xe as libc::c_int as libc::c_uchar,
    );
    port_byte_out(
        0x3d5 as libc::c_int as libc::c_ushort,
        (offset >> 8 as libc::c_int) as libc::c_uchar,
    );
    port_byte_out(
        0x3d4 as libc::c_int as libc::c_ushort,
        0xf as libc::c_int as libc::c_uchar,
    );
    port_byte_out(
        0x3d5 as libc::c_int as libc::c_ushort,
        (offset & 0xff as libc::c_int) as libc::c_uchar,
    );
}
#[no_mangle]
pub unsafe extern "C" fn get_cursor() -> libc::c_int {
    port_byte_out(
        0x3d4 as libc::c_int as libc::c_ushort,
        0xe as libc::c_int as libc::c_uchar,
    );
    let mut offset: libc::c_int =
        (port_byte_in(0x3d5 as libc::c_int as libc::c_ushort) as libc::c_int) << 8 as libc::c_int;
    port_byte_out(
        0x3d4 as libc::c_int as libc::c_ushort,
        0xf as libc::c_int as libc::c_uchar,
    );
    offset += port_byte_in(0x3d5 as libc::c_int as libc::c_ushort) as libc::c_int;
    return offset * 2 as libc::c_int;
}
#[no_mangle]
pub unsafe extern "C" fn set_char_at_video_memory(
    mut character: libc::c_char,
    mut offset: libc::c_int,
) {
    let mut video_memory: *mut libc::c_uchar = 0xb8000 as libc::c_int as *mut libc::c_uchar;
    *video_memory.offset(offset as isize) = character as libc::c_uchar;
    *video_memory.offset((offset + 1 as libc::c_int) as isize) =
        0x7 as libc::c_int as libc::c_uchar;
}
#[no_mangle]
pub unsafe extern "C" fn get_row_from_offset(mut offset: libc::c_int) -> libc::c_int {
    return offset / (2 as libc::c_int * 80 as libc::c_int);
}
#[no_mangle]
pub unsafe extern "C" fn get_offset(mut col: libc::c_int, mut row: libc::c_int) -> libc::c_int {
    return 2 as libc::c_int * (row * 80 as libc::c_int + col);
}
#[no_mangle]
pub unsafe extern "C" fn move_offset_to_new_line(mut offset: libc::c_int) -> libc::c_int {
    return get_offset(
        0 as libc::c_int,
        get_row_from_offset(offset) + 1 as libc::c_int,
    );
}
#[no_mangle]
pub unsafe extern "C" fn memory_copy(
    mut source: *mut libc::c_char,
    mut dest: *mut libc::c_char,
    mut nbytes: libc::c_int,
) {
    let mut i: libc::c_int = 0;
    i = 0 as libc::c_int;
    while i < nbytes {
        *dest.offset(i as isize) = *source.offset(i as isize);
        i += 1;
    }
}
#[no_mangle]
pub unsafe extern "C" fn scroll_ln(mut offset: libc::c_int) -> libc::c_int {
    memory_copy(
        (get_offset(0 as libc::c_int, 1 as libc::c_int) + 0xb8000 as libc::c_int)
            as *mut libc::c_char,
        (get_offset(0 as libc::c_int, 0 as libc::c_int) + 0xb8000 as libc::c_int)
            as *mut libc::c_char,
        80 as libc::c_int * (25 as libc::c_int - 1 as libc::c_int) * 2 as libc::c_int,
    );
    let mut col: libc::c_int = 0 as libc::c_int;
    while col < 80 as libc::c_int {
        set_char_at_video_memory(
            ' ' as i32 as libc::c_char,
            get_offset(col, 25 as libc::c_int - 1 as libc::c_int),
        );
        col += 1;
    }
    return offset - 2 as libc::c_int * 80 as libc::c_int;
}
#[no_mangle]
pub unsafe extern "C" fn print_string(mut string: *mut libc::c_char) {
    let mut offset: libc::c_int = get_cursor();
    let mut i: libc::c_int = 0 as libc::c_int;
    while *string.offset(i as isize) as libc::c_int != 0 as libc::c_int {
        if offset >= 25 as libc::c_int * 80 as libc::c_int * 2 as libc::c_int {
            offset = scroll_ln(offset);
        }
        if *string.offset(i as isize) as libc::c_int == '\n' as i32 {
            offset = move_offset_to_new_line(offset);
        } else {
            set_char_at_video_memory(*string.offset(i as isize), offset);
            offset += 2 as libc::c_int;
        }
        i += 1;
    }
    set_cursor(offset);
}
#[no_mangle]
pub unsafe extern "C" fn clear_screen() {
    let mut i: libc::c_int = 0 as libc::c_int;
    while i < 80 as libc::c_int * 25 as libc::c_int {
        set_char_at_video_memory(' ' as i32 as libc::c_char, i * 2 as libc::c_int);
        i += 1;
    }
    set_cursor(get_offset(0 as libc::c_int, 0 as libc::c_int));
}
#[no_mangle]
pub unsafe extern "C" fn clear_screen2(mut message: *const libc::c_char) {
    let mut i: libc::c_int = 0 as libc::c_int;
    while i < 5 as libc::c_int {
        let mut text: libc::c_char = *message.offset(i as isize);
        set_char_at_video_memory(text, i * 2 as libc::c_int);
        i += 1;
    }
    set_cursor(get_offset(0 as libc::c_int, 0 as libc::c_int));
}
