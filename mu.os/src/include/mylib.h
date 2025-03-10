#pragma once
#include <time.h>
#include <stdio.h>

// External variables
extern char ROOT_PATH[];
extern char START_PATH[];
extern char current_directory[];  // Ensure it's an array, not a pointer to a pointer

// Maximum path length
#define MAX_PATH_LENGTH 256

// Function declarations
extern void init_paths();

extern unsigned long _syscall(int num, void *a0, void *a1, void *a2, void *a3, void *a4, void *a5);
extern unsigned long sys_open(char *fn, int flags);
extern unsigned long sys_read(unsigned long fd, char *buff, unsigned long size);
extern unsigned long sys_reboot();
extern unsigned long sys_nanosleep(struct timespec *req, struct timespec *rem);

extern unsigned long str_len(char *sz);
extern void str_print(const char *format, ...);

extern void delay(int ticks);
extern void sleep_sec(int sec);

extern int str_eq(char *a, char *b);
extern unsigned long sys_write(unsigned long fd, char *buf, unsigned long len);
extern long sys_fork();
extern long sys_execve(char *filename, char **argv, char **envp);
extern int execute_process(char *filename);
extern int execute_process_with_args(const char *path, const char *arg, const char *arg2);

extern char* str_copy(char* dest, const char* src);

extern void console_open();
extern char console_read();
extern void console_write(char *str);
extern void console_write_char(char c);
extern void add_to_history(char *command);
extern int read_line(char *buff, int max);
extern void clear_shell();
extern void echo_command(char *buff);
extern int tokenize_input(char *line, char input[][1024], int max_tokens);

extern void dir(char path[1024]);
extern void Time();

// Directory functions
extern int change_directory(const char *path);
extern void init_paths();
extern void print_current_directory();
extern void pwd();
extern int ls(const char *path);
extern int dir_exists(const char *path); // Removed duplicate definition
extern int create_directory(const char *path);
extern int mkdir_create_folder(const char *folder_name);
extern void print_disk_usage(const char *path);
extern void lsblk();
extern void print_block_device_info(const char *device_name);
