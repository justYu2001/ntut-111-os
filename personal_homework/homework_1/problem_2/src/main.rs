use std::io;

fn main() {
    println!("Please input a number:");

    let mut input = String::new();

    io::stdin()
        .read_line(&mut input)
        .expect("Failed to read from stdin");

    let trimmed_input = input.trim();

    match trimmed_input.parse::<u32>() {
        Ok(number) => print_sequence(number),
        Err(error) => panic!("{error}"),
    }
}

fn print_sequence(start_number: u32) {
    let mut current_number = start_number;
    let mut sequence = vec![current_number];

    // Call `fork()` in the C library `unistd.h` to create a child process.
    let mut process_id = unsafe { libc::fork() };

    if process_id < 0 {
        panic!("Failed to fork!");
    }

    if process_id == 0 {
        while current_number > 1 {
            if current_number % 2 == 0 {
                current_number = current_number / 2;
            } else {
                current_number = 3 * current_number + 1;
            }

            sequence.push(current_number);
        }

        println!("{:?}", sequence);
        unsafe { libc::exit(0) };
    } else {
        unsafe { libc::wait(&mut process_id) };
    }
}