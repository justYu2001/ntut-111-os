use std::{fs::File, os::fd::AsRawFd, io::Write};

use clap::Parser;

#[derive(Parser)]
struct Args {
    source: std::path::PathBuf,
    destination: std::path::PathBuf,
}

fn main() {
    let args = Args::parse();

    let source_file = File::open(args.source).expect("Failed to read source file");
    let mut destination_file = File::create(args.destination).expect("Failed to create destination file");

    let mut buffer: [u8; 4096] = [0; 4096];

    let source_fd = source_file.as_raw_fd();
    let mut bytes_has_read = unsafe {
        // Call `read()` in the C library unistd.h to read a file.
        libc::read(
            source_fd, 
            buffer.as_mut_ptr() as *mut libc::c_void,
            buffer.len()
        )
    };

    while bytes_has_read > 0 {
        let bytes_to_write = unsafe {
            // Call `write()` in the C library unistd.h to write a file.
            libc::write(
                destination_file.as_raw_fd(),
                buffer.as_mut_ptr() as *const libc::c_void,
                bytes_has_read as usize
            )
        };

        if bytes_has_read != bytes_to_write {
            panic!("Failed to copy file");
        }

        bytes_has_read = unsafe {
            libc::read(
                source_fd, 
                buffer.as_mut_ptr() as *mut libc::c_void,
                buffer.len()
            )
        };
    }

    destination_file.flush().expect("Faile to flush destination file");
}
