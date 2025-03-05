#include <stdio.h>
#include "/mnt/myos/src/include/mylib.h" // Include your custom library

void pwd() {
    const char *cwd = print_current_directory(); // Use the function from mylib.c
    if (cwd) {
        printf("%s\n", cwd);
    } else {
        perror("pwd");
    }
}

int main() {
    pwd();
    return 0;
}


