#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    char *user = getenv("USER");
    if (user) {
        printf("%s\n", user);
    } else {
        printf("Unknown user\n");
    }
    return 0;
}

