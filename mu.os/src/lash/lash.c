#include <mylib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>  // For waitpid()
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

int main() {
    setuid(0); // Set root privileges
    setgid(0);
    
    if (getuid() == 0) {
        printf("lash is running as root! (UID: %d)\n", getuid());
    } else {
        printf("Failed to set root privileges! (UID: %d)\n", getuid());
    }

    init_paths();  // Initialize paths
    printf("\033[H\033[J"); // Clear screen
    str_print("LASH v0.2.0\n");

    console_open();

    while (1) {
        str_print("%s :> ", current_directory);

        char buff[1024];
        char tokens[10][1024];
        int len = read_line(buff, sizeof(buff));
        int token_count = tokenize_input(buff, tokens, 10);

        if (len == 0) continue;

        if (str_eq(tokens[0], "exit")) {
            str_print("Exiting shell...\n");
            break;
        } else if (str_eq(tokens[0], "reboot")) {
            str_print("\n\n*** SYSTEM REBOOTING ***\n");
            sys_reboot();
        } else if (str_eq(tokens[0], "clear")) {
            clear_shell();
        } else if (str_eq(tokens[0], "cd")) {
            if (token_count > 1) {
                if (change_directory(tokens[1]) == 0) {
                    str_print("Changed directory to: %s\n", current_directory);
                } else {
                    str_print("Error: Failed to change directory.\n");
                }
            } else {
                const char *home_dir = getenv("HOME");
                if (home_dir != NULL) {
                    if (change_directory(home_dir) == 0) {
                        str_print("Changed directory to home: %s\n", current_directory);
                    } else {
                        str_print("Error: Failed to change to home directory.\n");
                    }
                } else {
                    str_print("Error: HOME environment variable not set.\n");
                }
            }
        } else if (str_eq(tokens[0], "pwd")) {
            print_current_directory();
        } else if (str_eq(tokens[0], "ls")) {
            execute_process("/bin/ls");
        } else if (str_eq(tokens[0], "mkdir")) {
            if (token_count > 1) {
                execute_process_with_args("/bin/mkdir", tokens[1], NULL);
            } else {
                str_print("Error: Missing directory name for 'mkdir'.\n");
            }
        } else if (str_eq(tokens[0], "df")) {
            execute_process("/bin/df");
        } else if (str_eq(tokens[0], "lsblk")) {
            execute_process("/bin/lsblk");
        } else if (str_eq(tokens[0], "adduser")) {
            if (token_count > 1) {
                pid_t pid = fork();
                if (pid == 0) {
                    char *args[] = {"/bin/useradd", tokens[1], NULL};
                    execv(args[0], args);
                    perror("Error executing useradd");
                    exit(1);
                } else if (pid > 0) {
                    int status;
                    waitpid(pid, &status, 0);
                } else {
                    perror("Error: Failed to fork process");
                }
            } else {
                str_print("Error: Missing username for 'adduser'.\n");
            }
        } else if (str_eq(tokens[0], "login")) {
            if (token_count > 1) {
                execute_process_with_args("/bin/login", tokens[1], NULL);
            } else {
                str_print("Error: Missing username for 'login'.\n");
            }
        } else if (str_eq(tokens[0], "whoami")) {
            execute_process("/bin/whoami");
        } else if (str_eq(tokens[0], "logout")) {
            str_print("Logging out...\n");
            exit(0);
        } else {
            str_print("Command not found: %s\n", tokens[0]);
        }
    }

    return 0;
}
