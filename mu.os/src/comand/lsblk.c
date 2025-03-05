

#include <syscall.h>
#include <fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<string.h>
#include <stdarg.h>
#include <unistd.h>  
#include <sys/stat.h> 
#include<dirent.h>
#include <sys/types.h>
#include <sys/statvfs.h>


void print_block_device_info(const char *device_name) {
    char path[256];
    snprintf(path, sizeof(path), "/sys/block/%s/size", device_name);

    FILE *file = fopen(path, "r");
    if (file == NULL) {
        perror("Failed to open device size file");
        return;
    }

    // Read size in 512-byte sectors
    unsigned long long size_in_sectors;
    fscanf(file, "%llu", &size_in_sectors);
    fclose(file);

    // Convert size to bytes
    unsigned long long size_in_bytes = size_in_sectors * 512;

    // Convert size to gigabytes (GB)
    double size_in_gb = (double)size_in_bytes / (1024 * 1024 * 1024);

    printf("Device: %s\n", device_name);
    printf("Size: %.2f GB\n", size_in_gb);
    printf("\n");
}

// Function to list block devices
void lsblk() {
    DIR *dir = opendir("/sys/block");
    if (dir == NULL) {
        perror("Failed to open /sys/block");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        // Ignore . and ..
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // Print information for each block device
        print_block_device_info(entry->d_name);
    }

    closedir(dir);
}

int main(){
    lsblk();
}

