#include <stdio.h>
#include <sys/statvfs.h>

#define BYTES_IN_GB (1024.0 * 1024 * 1024) // 1 GB in bytes

void print_disk_usage(const char *path) {
    struct statvfs stat;

    if (statvfs(path, &stat) != 0) {
        perror("statvfs");
        return;
    }

    // Compute sizes in bytes
    unsigned long long total_size = (unsigned long long)stat.f_blocks * stat.f_frsize;
    unsigned long long free_size = (unsigned long long)stat.f_bfree * stat.f_frsize;
    unsigned long long available_size = (unsigned long long)stat.f_bavail * stat.f_frsize;
    unsigned long long used_size = total_size - free_size;
    unsigned long long data_non_root_user = available_size; // Corrected

    // Convert to GB
    double total_size_gb = total_size / BYTES_IN_GB;
    double used_size_gb = used_size / BYTES_IN_GB;
    double free_size_gb = free_size / BYTES_IN_GB;
    double available_size_gb = available_size / BYTES_IN_GB;
    double data_non_root_user_gb = data_non_root_user / BYTES_IN_GB;

    // Print results
    printf("Filesystem: %s\n", path);
    printf("Total space: %.2f GB\n", total_size_gb);
    printf("Used space: %.2f GB\n", used_size_gb);
    printf("Free space: %.2f GB\n", free_size_gb);
    printf("Available space: %.2f GB\n", available_size_gb);
    printf("Available non-root user space: %.2f GB\n", data_non_root_user_gb);
}

int main() {
    print_disk_usage("/");
    return 0;
}

