#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("Logging out...\n");
    system("pkill -KILL -u $USER"); // Kills all user processes
    return 0;
}

