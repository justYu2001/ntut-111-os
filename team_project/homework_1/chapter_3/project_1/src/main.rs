use std::ffi::{CString, c_char};
use std::io::{self, Write};

use regex::Regex;

const MAX_LINE:u8 = 80;
const MAX_LENGTH_OF_ARGS: u8 = MAX_LINE / 2 + 1;

fn main() {
    let should_run = true;
    let mut history : Vec<String>= Vec::new();

    while should_run {
        let mut input = String::new();

        print!("osh>");
        io::stdout().flush().unwrap();

        io::stdin()
            .read_line(&mut input)
            .expect("Failed to read from stdin");

        let number_of_args = input.split(" ").collect::<Vec<&str>>().len();

        if number_of_args > MAX_LENGTH_OF_ARGS.into() {
            panic!("Command is too long. The limit on command words is {}", MAX_LENGTH_OF_ARGS);
        }

        let trimmed_input = input.trim();

        if trimmed_input == "history" {
            for (index, record) in history.iter().enumerate() {
                println!("{}: {}", index + 1, record);
            }

            continue;
        }

        let regex = Regex::new(r"^history [1-9][0-9]*$").unwrap();
        if regex.is_match(trimmed_input) {
            let (_, command_index_string) = trimmed_input.rsplit_once(" ").unwrap();
            let command_index = command_index_string.parse::<usize>().unwrap();
            execute_commands(&history[command_index - 1]);
            continue;
        }

        if trimmed_input == "exit" {
            std::process::exit(0);
        }

        history.push(trimmed_input.to_string());

        execute_commands(&trimmed_input);
    }
}

fn execute_commands(input: &str) {
    let commands:Vec<&str> = input.split("&").collect();

    for command in commands {
        let words: Vec<&str> = command.split_whitespace().collect();

        let arguments: Vec<CString> = words.iter().map(|&word| {
            CString::new(word).unwrap()
        }).collect();

        let mut argument_pointers: Vec<*const c_char> = arguments.iter().map(|argument| {
            argument.as_ptr()
        }).collect();

        argument_pointers.push(0 as *const c_char);

        let mut process_id = unsafe { libc::fork() };

        if process_id < 0 {
            panic!("Failed to create a child process");
        }

        if process_id == 0 {
            let result = unsafe { libc::execvp(arguments[0].as_ptr(), argument_pointers.as_ptr()) };

            if result == -1 {
                panic!("Failed to execute command {}", command);
            }

            unsafe { libc::exit(0) };
        } else {
            unsafe { libc::wait(&mut process_id) };
        }
    }
}