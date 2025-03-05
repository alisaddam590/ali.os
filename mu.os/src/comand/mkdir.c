#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define MAX_PATH 1024

// Function to check if a directory exists
int dir_exists(const char *path) {
    struct stat st;
    return (stat(path, &st) == 0 && S_ISDIR(st.st_mode));
}

// Function to create a directory
int create_directory(const char *path) {
    return mkdir(path, 0755); // Permissions: rwxr-xr-x
}

// Function to create a folder in the current directory if it does not exist
int mkdir_create_folder(const char *folder_name) {
    char current_path[MAX_PATH];

    // Get the current working directory
    if (getcwd(current_path, sizeof(current_path)) == NULL) {
        perror("getcwd failed");
        return -1;
    }

    // Construct the full directory path
    char folder_path[MAX_PATH];
    snprintf(folder_path, sizeof(folder_path), "%s/%s", current_path, folder_name);

    // Check if the directory already exists
    if (dir_exists(folder_path)) {
        printf("Error: Directory '%s' already exists in '%s'.\n", folder_name, current_path);
        return -1;
    }

    // Create the directory
    if (create_directory(folder_path) != 0) {
        printf("Error: Could not create directory '%s' in '%s': %s\n", folder_name, current_path, strerror(errno));
        return -1;
    }

    printf("Directory '%s' created successfully in '%s'.\n", folder_name, current_path);
    return 0;
}

int main(int argc, char *argv[]) {
    // Check if folder name is provided
    if (argc != 2) {
        printf("Usage: %s <folder_name>\n", argv[0]);
        return 1;
    }

    // Get the folder name from command-line argument
    const char *folder_name = argv[1];

    // Validate folder name
    if (strlen(folder_name) == 0) {
        printf("Invalid folder name.\n");
        return 1;
    }

    // Create the folder
    return mkdir_create_folder(folder_name);
}
