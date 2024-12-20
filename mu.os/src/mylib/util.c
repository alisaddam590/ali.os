#include <mylib.h>
#include <syscall.h>
#include <fcntl.h>

#define HISTORY_SIZE 10

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

void str_print(char *str) {
    _syscall(SYS_write, (void *)1 /*stdout*/, str, (void *)str_len(str), 0, 0, 0);
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



