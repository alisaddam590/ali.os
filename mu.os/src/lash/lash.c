#include <mylib.h>
#include <fcntl.h>

unsigned long console_fd = 0;

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

int read_line(char *buff, int max) {
    int i = 0;

    while (i < max - 1) {
        char c = console_read();

        if (c == 0) {
            continue;
        }

        if (c == '\b' || c == 127) { // Backspace or DEL
            if (i > 0) {
                console_write("\b \b"); // Erase character visually
                i--;
            }
        } else if (c == '\n' || c == '\r') { // End of input
            console_write_char('\n');
            buff[i] = 0; // Null-terminate
            return i;
        } else {
            console_write_char(c); // Echo character
            buff[i++] = c;
        }
    }

    buff[max - 1] = 0; // Ensure null-termination
    return i;
}

int main() {
    str_print("\033[H\033[J"); // Clear screen
    str_print("LASH v0.1.0\n");
    str_print(" :> ");

    console_open();

    while (1) {
        char buff[1024];
        int len = read_line(buff, sizeof(buff));

        if (len == 0) {
            continue;
        }

        str_print("INPUT: ");
        str_print(buff);
        str_print("\n");

        if (str_eq(buff, "reboot")) {
            str_print("\n\n*** SYSTEM REBOOTING ***\n");
            sys_reboot();
        } else if (str_eq(buff, "exit")) {
            str_print("Exiting shell...\n");
            break;
        }

        str_print(" :> ");
    }

    return 0;
}