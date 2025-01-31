#include <mylib.h>
#include <unistd.h>
#include<stdio.h>
#include<time.h>


int main() {
    printf("\033[H\033[J"); // Clear screen
    printf("LASH v0.2.0\n");
   

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
            echo_command(buff);     // Call echo function to display the argument
        }else if(str_eq(tokens[0],"time")){
            Time();
        }
    }
    return 0;
}


