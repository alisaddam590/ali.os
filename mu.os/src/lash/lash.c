#include <mylib.h>

int main() {
    str_print("\033[H\033[J"); // Clear screen
    str_print("LASH v0.2.0\n");

    str_copy(cwd,"/user");
    console_open();

    while (1) {
        str_print(cwd);
        str_print(" :> ");
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
        } 
        // Check for reboot command
        else if (str_eq(tokens[0], "reboot")) {
            str_print("\n\n*** SYSTEM REBOOTING ***\n");
            sys_reboot();
        } 
        // Check for clear command
        else if (str_eq(tokens[0], "clear")) {
            clear_shell();
        } 
        // Check for echo command
        else if (str_eq(tokens[0], "echo")) {
            echo_command(buff);  // Call echo function to display the argument
        } 
        // Check for cd command
         else if (str_eq(tokens[0], "cd")) {
            cd_command(tokens[1], token_count);  // Call cd_command function
        }

        else if (str_eq(tokens[0], "pwd")) {
            str_print(cwd);
        }
        else if (str_eq(tokens[0], "mkdir")) {
            mkdir_command(tokens[1], token_count);  // Call mkdir_command function
        }
    }

    return 0;
}
