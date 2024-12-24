#pragma once
#include <time.h>

extern char history[10][1024];  // Store up to 10 commands
extern int history_index;            // The index where the next command will be stored
extern unsigned long console_fd;
extern char cwd[1024]; 

extern unsigned long _syscall(int num, void *a0, void *a1, void *a2, void *a3, void *a4, void *a5);

extern unsigned long sys_open(char *fn, int flags);
extern unsigned long sys_read(unsigned long fd, char *buff, unsigned long size);
extern unsigned long sys_reboot();
extern  long sys_nanosleep(struct timespec *req, struct timespec *rem);

extern unsigned long str_len(char *sz);
extern void str_print(char *str);

extern void delay(int ticks);
extern void sleep_sec(int sec);

extern int str_eq(char *a, char *b);
extern unsigned long sys_write(unsigned long fd, char *buf, unsigned long len);
extern long sys_fork();
extern long sys_execve(char *filename, char **argv, char **envp);
extern int execute_process(char *filename);



extern char* str_copy(char* dest, const char* src);


extern void console_open() ;

extern char console_read() ;
extern void console_write(char *str) ;
extern void console_write_char(char c) ;
extern void add_to_history(char *command) ;
extern int read_line(char *buff, int max) ;
extern void clear_shell();
extern void echo_command(char *buff) ;
extern int tokenize_input(char *line, char input[][1024], int max_tokens) ;
extern void pwd_command () ;
extern long sys_chdir(char *path);
extern unsigned long sys_getcwd(char *buf, unsigned long size);
