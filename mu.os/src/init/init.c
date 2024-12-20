#include <syscall.h>
#include <fcntl.h>
#include <mylib.h>



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
        str_print("\n");
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
