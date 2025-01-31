#include <syscall.h>
#include <fcntl.h>
#include <mylib.h>

// Function to split a line into tokens based on spaces
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


int main() {
    sleep_sec(1);

    str_print("\033[H\033[J");

    char *msg = "MyOS 0.0.0.2 Initializing...\n";

    sleep_sec(1);

    str_print(msg);

    //////////////////////////////////

char line[] = "  echo   this is a   test  ";
    char tokens[10][1024];
    int token_count = tokenize_input(line, tokens, 10);



    for (int i = 0; i < token_count; i++) {

        str_print(tokens[i]);
    }

    ///////////////////////////////

    sleep_sec(3);

    execute_process("/bin/lash");

    while(1) {
        //event loop, for now just tick...
        sleep_sec(1);
    }

    return 0;
}



