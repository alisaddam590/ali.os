#include <mylib.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

int main() {
    printf("\033[H\033[J"); // Clear screen
    str_print("LASH v0.2.0\n");

    console_open();

    while (1) {
        // Get the current directory and print it as the prompt
        const char *path = print_current_directory();
        str_print("%s :>", path); // Print the current path before the prompt

        char buff[1024];
        char tokens[10][1024];
        int len = read_line(buff, sizeof(buff));
        int token_count = tokenize_input(buff, tokens, 10);

        if (len == 0) {
            continue;
        }

        // Check for exit command
        if (str_eq(tokens[0], "exit")) {
            str_print("Exiting shell...\n");
            break;
        } else if (str_eq(tokens[0], "reboot")) {
            str_print("\n\n*** SYSTEM REBOOTING ***\n");
            sys_reboot();
        } else if (str_eq(tokens[0], "clear")) {
            clear_shell();
        } else if (str_eq(tokens[0], "echo")) {
            // Handle echo command: print the rest of the input
            echo_command(buff);  // Call echo function to display the argument
        } else if (str_eq(tokens[0], "time")) {
            Time();
        } else if (str_eq(tokens[0], "cd")) {
            if (token_count > 1) {
                // Call the change directory function with the specified path
                if (change_directory(tokens[1]) == 0) {
                    str_print("Directory changed to: ");
                    // Print the current directory after change
                    str_print(print_current_directory());
                    str_print("\n");
                } else {
                    str_print("Error: Failed to change directory.\n");
                }
            } else {
                str_print("Error: Missing directory argument for 'cd'.\n");
            }
        } else if (str_eq(tokens[0], "pwd")) {
            pwd();
        } else if (str_eq(tokens[0], "ls")) {
            ls(path);
        } else if (str_eq(tokens[0], "mkdir")) {
            if (token_count > 1) {
                // Call the mkdir_create_folder function with the folder name
                if (mkdir_create_folder(tokens[1]) == 0) {
                    str_print("Directory '%s' created successfully in the current directory.\n", tokens[1]);
                } else {
                    str_print("Failed to create directory.\n");
                }
            } else {
                str_print("Error: Missing folder argument for 'mkdir'.\n");
            }
        }
    }

    return 0;
}

