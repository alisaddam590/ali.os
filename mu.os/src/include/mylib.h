#pragma once
#include <time.h>
#include<stdio.h>

extern unsigned long _syscall(int num, void *a0, void *a1, void *a2, void *a3, void *a4, void *a5);

extern unsigned long sys_open(char *fn, int flags);
extern unsigned long sys_read(unsigned long fd, char *buff, unsigned long size);
extern unsigned long sys_reboot();
unsigned long sys_nanosleep(struct timespec *req, struct timespec *rem);

extern unsigned long str_len(char *sz);
extern void str_print(const char *format, ...);

extern void delay(int ticks);
extern void sleep_sec(int sec);

extern int str_eq(char *a, char *b);
extern unsigned long sys_write(unsigned long fd, char *buf, unsigned long len);
extern long sys_fork();
extern long sys_execve(char *filename, char **argv, char **envp);
extern int execute_process(char *filename);



char* str_copy(char* dest, const char* src);


void console_open() ;

char console_read() ;
void console_write(char *str) ;
void console_write_char(char c) ;
void add_to_history(char *command) ;
int read_line(char *buff, int max) ;
void clear_shell();
void echo_command(char *buff) ;
int tokenize_input(char *line, char input[][1024], int max_tokens) ;

extern void dir(char path[1024]);
extern void Time();
extern int change_directory(const char *path);
extern char* print_current_directory();
extern void pwd();
extern int ls(const char *path);
extern int dir_exists(const char *path);
extern int create_directory(const char *path);
extern int mkdir_create_folder(const char *folder_name);
