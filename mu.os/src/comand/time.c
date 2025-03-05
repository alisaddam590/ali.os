#include <stdio.h>
#include <time.h>

void Time() {
    time_t t = time(NULL);
    if (t == -1) {
        printf("Error: Unable to get current time\n");
        return;
    }

    struct tm *localTime = localtime(&t);
    if (localTime == NULL) {
        printf("Error: Unable to convert time to local time\n");
        return;
    }

    printf("Current time: %s", asctime(localTime));
}

int main() {
    Time();  // Call the Time function to display the current time
    return 0;
}

