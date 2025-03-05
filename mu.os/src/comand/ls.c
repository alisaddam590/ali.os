#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_PATH_LENGTH 1024

int ls(const char *path) {
    DIR *dir;
    struct dirent *entry;
    char full_path[MAX_PATH_LENGTH];

    // If no path is provided, use the current directory (".")
    if (path == NULL || strlen(path) == 0) {
        path = ".";
    }

    // Ensure full_path is safely set
    strncpy(full_path, path, MAX_PATH_LENGTH - 1);
    full_path[MAX_PATH_LENGTH - 1] = '\0';  // Ensure string termination

    // Open the directory
    dir = opendir(full_path);
    if (dir == NULL) {
        printf("Error: Unable to open directory\n");
        return -1;
    }

    // Read and list the contents of the directory
    printf("Listing contents of %s:\n", full_path);
    while ((entry = readdir(dir)) != NULL) {
        // Skip the special "." and ".." entries
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        printf("%s\n", entry->d_name);
    }

    // Close the directory
    closedir(dir);

    return 0;
}

int main() {
    char cwd[MAX_PATH_LENGTH];

    // Get current working directory
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        printf("Error: Unable to get current directory\n");
        return -1;
    }

    // Call ls function to list contents of current directory
    return ls(cwd);
}

