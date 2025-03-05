#include <stdio.h>
#include <string.h>

void echo_command(char *buff) {
    if (buff == NULL || strlen(buff) == 0) {
        printf("\n");
        return;
    }
    
    printf("\n%s\n", buff); // Print the echo string directly
}

int main() {
    char input[1024];

    // Prompt the user
    printf("Enter text to echo: ");
    
    // Read user input
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }

    // Remove the trailing newline character
    input[strcspn(input, "\n")] = 0;

    // Call the echo function
    echo_command(input);

    return 0;
}

