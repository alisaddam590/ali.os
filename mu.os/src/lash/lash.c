#include <mylib.h>


int main() {
    str_print("\033[H\033[J"); // Clear screen
    str_print("LASH v0.2.0\n");
   

    console_open();

    while (1) {
        str_print(" :> ");
        char buff[1024];
        char tokens[10][1024];
        int len = read_line(buff, sizeof(buff));

        int token_count = tokenize_input(buff, tokens, 10);
        if (len == 0) {
            continue;
        }

        // Check for echo command
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
        }else if (str_eq(tokens[0], "cd")) {
             if (token_count > 1 && token_count <3) {
            char *new_path = tokens[1];

            if (sys_chdir(new_path) == 0) {
                str_print("Directory changed to: ");
                str_print(new_path);
                str_print("\n");
                str_copy(cwd, new_path);  // Optionally update cwd buffer
            } else {
                str_print("cd: No such file or directory: ");
                str_print(new_path);
                str_print("\n");
                }   }
            else  {
            str_print("cd: Missing argument\n");
            }       
        }
    }
    return 0;
}
