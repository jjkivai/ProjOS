use std::arch::asm;

use libc::c_void;

#[link(name = "./drivers/display.h")]
extern "C" {
    fn lol() -> c_void;
}
fn main() {
    println!("lol");
    unsafe {
        lol();
    }
}

// pub unsafe extern "C" fn port_byte_in(mut port: libc::c_ushort) -> libc::c_uchar {
//     let mut result: libc::c_uchar = 0;
//     asm!(
//         "in %dx, %al", lateout("ax") result, inlateout("dx") port => _,
//         options(preserves_flags, pure, readonly, att_syntax)
//     );
//     return result;
// }
