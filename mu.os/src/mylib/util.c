#include <mylib.h>
#include <syscall.h>
#include <fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<string.h>
#include <stdarg.h>
#include <unistd.h>  
#include <sys/stat.h> 
#include<dirent.h>
#include <sys/types.h>


#define HISTORY_SIZE 10
#define MAX_PATH_LENGTH 256

char current_directory[MAX_PATH_LENGTH] = "//user/ali";  
char root_directory[MAX_PATH_LENGTH] = "/";



char history[HISTORY_SIZE][1024];  // Store up to 10 commands
int history_index = 0;             // The index where the next command will be stored
unsigned long console_fd = 0;



unsigned long str_len(char *sz) {
    int count = 0;

    while(*sz++) {
        count++;
    }

    return count;
}

void delay(int ticks) {
    for (int i=0; i<ticks; i++) {
        //nothing...
    }
}

void str_print(const char *format, ...) {
    va_list args;
    va_start(args, format);

    // Create a buffer to store the final string
    char buffer[1024];
    int len = vsnprintf(buffer, sizeof(buffer), format, args);  // Format the string with the arguments

    // If the formatting is successful, write to stdout
    if (len >= 0) {
        _syscall(SYS_write, (void *)1 /*stdout*/, buffer, (void *)len, 0, 0, 0);
    }

    va_end(args);
}

int str_eq(char *a, char *b) {
    char ca = *a;
    char cb = *b;

    for (; ca != 0 && cb != 0; ca = *(++a), cb = *(++b)) {
        if (ca != cb) {
            return 0;
        }
    }

    return ca == cb;
}


char* str_copy(char* dest, const char* src) {
    char* orig_dest = dest;

    // Copy characters from src to dest until the null terminator is encountered
    while (*src != '\0') {
        *dest = *src;
        dest++;
        src++;
    }

    // Null-terminate the destination string
    *dest = '\0';

    return orig_dest;  // Return the original destination pointer
}


void console_open() {
    console_fd = sys_open("/dev/console", O_RDWR | O_NDELAY);
}

char console_read() {
    static char buffer[128];
    static int buf_index = 0, buf_size = 0;

    if (buf_index >= buf_size) {
        buf_size = sys_read(console_fd, buffer, 1);
        if (buf_size <= 0) {
            struct timespec ts = { .tv_sec = 0, .tv_nsec = 10000 };
            sys_nanosleep(&ts, NULL);
            return 0;
        }
        buf_index = 0;
    }

    return buffer[buf_index++];
}

void console_write(char *str) {
    sys_write(console_fd, str, str_len(str));
}

void console_write_char(char c) {
    sys_write(console_fd, &c, 1);
}

void add_to_history(char *command) {
    if (history_index < HISTORY_SIZE) {
        str_copy(history[history_index], command);
        history_index++;
    } else {
        // If history is full, shift all commands up and add the new one
        for (int i = 1; i < HISTORY_SIZE; i++) {
            str_copy(history[i-1], history[i]);
        }
        str_copy(history[HISTORY_SIZE - 1], command);
    }
}

int read_line(char *buff, int max) {
    int history_pos = 0;               // The current position in history for navigation
    int pointer_place = 0;
    int i = 0;
    history_pos = history_index;  // Start at the latest command in history
    pointer_place = 0; // Start pointer position at 0

    while (i < max - 1) {
        char c = console_read();

        if (c == 0) {
            continue;
        }

        if (c == '\b' || c == 127) { // Backspace or DEL
            if (i > 0) {
                console_write("\b \b"); // Erase character visually   
                i--;
                if (pointer_place > 0) {
                    pointer_place--;  // Move pointer left
                }
            }
        } else if (c == '\n' || c == '\r') { // End of input
            pointer_place = 0;
            console_write_char('\n');
            buff[i] = 0; // Null-terminate
            add_to_history(buff);  // Add command to history
            return i;
        } else if (c == '\e') { // Arrow keys
            c = console_read(); // Read next character

            if (c == '[') {
                c = console_read(); // Read final character for arrow key

                if (c == 'A') { // Up Arrow
                    if (history_pos > 0) {
                        history_pos--;
                        str_copy(buff, history[history_pos]);
                        i = str_len(buff); // Set buffer length to the length of the command
                        console_write("\r"); // Clear current line
                        console_write(" :> ");
                        console_write(buff); // Display previous command
                        pointer_place = i; // Update pointer position
                    }
                } else if (c == 'B') { // Down Arrow
                    if (history_pos < history_index) {
                        history_pos++;
                        str_copy(buff, history[history_pos]);
                        i = str_len(buff); // Set buffer length to the length of the command
                        console_write("\r"); // Clear current line
                        console_write(" :> ");
                        console_write(buff); // Display next command
                        pointer_place = i; // Update pointer position
                    }
                } else if (c == 'D') { // Left Arrow
                    if (pointer_place > 0) {
                        pointer_place--;
                        console_write("\b"); // Move cursor left
                    }
                } else if (c == 'C') { // Right Arrow
                    if (pointer_place < i) {
                        pointer_place++;
                        console_write("\033[C"); // Move cursor right
                    }
                }
            }
        } else {
            console_write_char(c); // Echo character
            buff[i++] = c;
            pointer_place++;  // Update pointer position
        }
    }

    buff[max - 1] = 0; // Ensure null-termination
    return i;
}

void clear_shell(){
        str_print("\033[H\033[J");
}

void echo_command(char *buff) {
    str_print("\n");
    str_print(buff); // Print the echo string directly
    str_print("\n"); 
}

int tokenize_input(char *line, char input[][1024], int max_tokens) {
    int token_count = 0;  // Number of tokens found
    int i = 0, j = 0;     // Indices for input array and current token

    while (*line != '\0' && token_count < max_tokens) {
        // Skip leading spaces (spaces, tabs, or newlines)
        while (*line == ' ' || *line == '\t' || *line == '\n') {
            line++;
        }

        // Read the next word
        while (*line != '\0' && *line != ' ' && *line != '\t' && *line != '\n') {
            input[token_count][j++] = *line++;
        }

        // Null-terminate the token if we have read a word
        if (j > 0) {
            input[token_count][j] = '\0';
            token_count++;
            j = 0;  // Reset for the next token
        }
    }

    return token_count;  // Return the number of tokens found
}



void Time() {
    time_t t;
    time(&t);
    struct tm *localTime = localtime(&t);
    printf("Current time: %s", asctime(localTime));
}

int change_directory(const char *path) {
    // Save the current directory as the previous one before changing
    strncpy(root_directory, current_directory, MAX_PATH_LENGTH - 1);
    root_directory[MAX_PATH_LENGTH - 1] = '\0'; // Ensure string termination

    // Basic input validation
    if (path == NULL || strlen(path) == 0) {
        str_print("Error: Path cannot be empty\n");
        return -1;
    }

    // Handle "cd /" (root directory)
    if (strcmp(path, "/") == 0) {
        strcpy(current_directory, "/");
        return 0;
    }

    // Handle "cd .." (go back to the previous directory)
    if (strcmp(path, "..") == 0) {
        // Make sure we are not at the root directory
        if (strcmp(current_directory, "/") != 0) {
            // Find the last occurrence of '/'
            char *last_slash = strrchr(current_directory, '/');
            if (last_slash != NULL) {
                // Set the character after the last '/' to '\0' to "cut off" the last directory name
                *last_slash = '\0';

                // Ensure we don't end up with an empty path (avoid cutting off everything)
                if (strlen(current_directory) == 0) {
                    strncpy(current_directory, "/", MAX_PATH_LENGTH - 1);
                }
            }
        } else {
            str_print("Error: Already at the root directory\n");
            return -1;
        }
        return 0;
    }

    // Check if the given path exists using stat()
    struct stat path_stat;
    char full_path[MAX_PATH_LENGTH];
    
    // If the path is relative, combine with the current_directory
    if (path[0] != '/') {
        snprintf(full_path, MAX_PATH_LENGTH, "%s/%s", current_directory, path);
    } else {
        strncpy(full_path, path, MAX_PATH_LENGTH - 1);
        full_path[MAX_PATH_LENGTH - 1] = '\0'; // Ensure string termination
    }

    if (stat(full_path, &path_stat) != 0) { // If stat fails, path doesn't exist
        str_print("Error: The path does not exist\n");
        return -1;
    }

    // If the path is a directory, proceed with changing directory
    if (S_ISDIR(path_stat.st_mode)) {
        strncpy(current_directory, full_path, MAX_PATH_LENGTH - 1);
        current_directory[MAX_PATH_LENGTH - 1] = '\0'; // Ensure string termination
    } else {
        str_print("Error: The path is not a directory\n");
        return -1;
    }

    // Ensure the path doesn't exceed the max length
    if (strlen(current_directory) >= MAX_PATH_LENGTH) {
        str_print("Error: Path is too long\n");
        return -1;
    }

    return 0;
}



// Function to return the current directory (instead of printing it directly)
char* print_current_directory() {
    return current_directory;
}

void pwd() {
    str_print("Current directory: %s\n", current_directory);
}

int ls(const char *path) {
    DIR *dir;
    struct dirent *entry;
    char full_path[MAX_PATH_LENGTH];

    // If no path is provided, use current_directory
    if (path == NULL || strlen(path) == 0) {
        path = current_directory;
    }

    // Build the full path (current directory + the given path)
    if (path[0] != '/') {
        snprintf(full_path, MAX_PATH_LENGTH, "%s/%s", current_directory, path);
    } else {
        strncpy(full_path, path, MAX_PATH_LENGTH - 1);
        full_path[MAX_PATH_LENGTH - 1] = '\0'; // Ensure string termination
    }

    // Open the directory
    dir = opendir(full_path);
    if (dir == NULL) {
        str_print("Error: Unable to open directory\n");
        return -1;
    }

    // Read and list the contents of the directory
    str_print("Listing contents:\n");
    while ((entry = readdir(dir)) != NULL) {
        // Skip the special "." and ".." entries
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // Print the name of each entry
        printf("%s\n", entry->d_name);
    }

    // Close the directory
    closedir(dir);

    return 0;
}

int dir_exists(const char *path) {
    struct stat statbuf;
    return (stat(path, &statbuf) == 0 && S_ISDIR(statbuf.st_mode)); // Check if path is a directory
}

// Pseudo code for creating a directory
int create_directory(const char *path) {
    int result = mkdir(path, 0755);  // Permissions: rwxr-xr-x (owner can read/write/exec, others can read/exec)
    if (result == -1) {
        perror("mkdir failed");  // Print the error message if mkdir fails
        return -1;  // Return -1 if directory creation fails
    }
    return 0; // Success
}

// The mkdir function that only creates a directory in the current directory
int mkdir_create_folder(const char *folder_name) {
    // Get the current directory path (you can get it from your shell environment)
    const char *current_path = print_current_directory();

    // Construct the full directory path (current path + folder name)
    char folder_path[1024];
    snprintf(folder_path, sizeof(folder_path), "%s/%s", current_path, folder_name);

    // Debugging: Print the full path being used
    printf("Attempting to create directory at path: %s\n", folder_path);

    // Check if the directory already exists
    if (dir_exists(folder_path)) {
        str_print("Error: Directory '%s' already exists in the current directory '%s'.\n", folder_name, current_path);
        return -1; // Directory exists, return failure
    }

    // Create the directory in the current directory
    if (create_directory(folder_path) != 0) {
        str_print("Error: Could not create directory '%s' in the current directory '%s'.\n", folder_name, current_path);
        return -1; // Directory creation failed
    }

    str_print("Directory '%s' created successfully in the current directory '%s'.\n", folder_name, current_path);
    return 0; // Success
}


