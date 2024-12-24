
#include<stdio.h>
#include <mylib.h>
#include <linux/fs.h> 



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
        }      // Check for cd command
         else if (str_eq(tokens[0], "cd")) {
            cd_command(tokens[1], token_count);  // Call cd_command function
        }
        else if (str_eq(tokens[0], "pwd")) {
            str_print(cwd);
        }
        else if(str_eq(tokens[0],"mkdir")){
             struct inode *user_path=resolve_path("/mnt/myos/src/user");
             char file_name[1024];
            str_print("Enter the name of the file :");
            while(str_eq(tokens[0],file_name)==0){
                str_print("Enter the name of the file :");
                str_eq(tokens[0],file_name);
            }
            sys_create_file(user_path, file_name);

           
           
            
        }
    }
    return 0;
}
