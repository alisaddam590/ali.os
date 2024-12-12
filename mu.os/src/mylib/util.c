#include <mylib.h>
#include <syscall.h>

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


char* str_tok(char *str, const char *delim) {
    static char *last = NULL;  // Static pointer to remember the last position in the string
    if (str == NULL) {
        str = last;  // If str is NULL, continue tokenizing the remaining string
    }

    // Skip leading delimiters
    while (*str) {
        const char *d = delim;
        int found_delim = 0;

        // Check if the current character is one of the delimiters
        while (*d) {
            if (*str == *d) {
                found_delim = 1;
                break;
            }
            d++;
        }

        if (!found_delim) {
            break;
        }

        str++;  // Move to the next character
    }

    if (*str == '\0') {
        last = NULL;  // If we've reached the end of the string, return NULL
        return NULL;
    }

    char *start = str;  // Mark the start of the token

    // Find the next delimiter or the end of the string
    while (*str) {
        const char *d = delim;
        int found_delim = 0;

        // Check if the current character is one of the delimiters
        while (*d) {
            if (*str == *d) {
                found_delim = 1;
                break;
            }
            d++;
        }

        if (found_delim) {
            *str = '\0';  // Null-terminate the token
            last = str + 1;  // Store the next position for the next token
            return start;  // Return the token
        }
        str++;
    }

    last = NULL;  // If we reached the end, return the last token
    return start;
}
