#include <stdio.h>
#include <string.h>

#define LINE_LENGTH 256
#define MAX_DIRECTORIES 32768
#define TARGET_SIZE 40000000

unsigned long long explore(unsigned long long **sizes) {
    char buffer[LINE_LENGTH];
    // Burn lines until we get to ls
    do {
        fgets(buffer, LINE_LENGTH, stdin);
    } while (strcmp(buffer, "$ ls\n"));
    unsigned long long dir_size = 0;
    unsigned short num_children = 0;
    unsigned long long file_size;
    while (1) {
        if (!fgets(buffer, LINE_LENGTH, stdin) || *buffer == '$') {
            break;
        }
        if (sscanf(buffer, "%llu", &file_size)) {
            dir_size += file_size;
        } else {
            num_children++;
        }
    }
    while (num_children--) {
        dir_size += explore(sizes);
    }
    *(*sizes)++ = dir_size;
    return dir_size;
}

int main() {
    // A binary search tree would be better but I don't really care
    static unsigned long long sizes[MAX_DIRECTORIES];
    // Zero at beginning of array is sentinel value when scanning backward at end
    static unsigned long long *write_sizes_ptr = sizes + 1;
    unsigned long long total_size = explore(&write_sizes_ptr);
    unsigned long long remove_size = total_size - TARGET_SIZE;
    unsigned long long closest_match = -1;
    while (*--write_sizes_ptr) {
        if (*write_sizes_ptr >= remove_size && *write_sizes_ptr < closest_match) {
            closest_match = *write_sizes_ptr;
        }
    }
    printf("%llu\n", closest_match);
    return 0;
}
