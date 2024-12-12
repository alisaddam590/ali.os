#include <syscall.h>
#include <fcntl.h>
#include <mylib.h>

int main() {

    str_print("\033[H\033[J");
     char input[] = "Hello, world! This is a test.";
    char *delim = " ,.!";  // Delimiters: space, comma, period, exclamation mark
    char *token = str_tok(input, delim);  // First call with the input string
    str_print(token);

    char *msg = "MyOS 0.0.0.2 Initializing...\n";

    



    sleep_sec(1);

    str_print(msg);

    sleep_sec(5);

    execute_process("/bin/lash");

    while(1) {
        //event loop, for now just tick...
        sleep_sec(1);
    }

    return 0;
}