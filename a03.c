#include <stdio.h>
#include <string.h>

#define LINE_SIZE 256

static inline int priority(char item) {
    return (item < 'a') ? item - 'A' + 27 : item - 'a' + 1;
}

int main() {
    char input_buffers[3][LINE_SIZE];
    char duplicate_items[LINE_SIZE];
    int totalscore = 0;
    while (fgets(input_buffers[0], LINE_SIZE, stdin)) {
        fgets(input_buffers[1], LINE_SIZE, stdin);
        fgets(input_buffers[2], LINE_SIZE, stdin);
        char *duplicatesptr = duplicate_items;
        for (char *scanptr = input_buffers[0]; *scanptr; scanptr++) {
            if (strchr(input_buffers[1], *scanptr)) {
                *duplicatesptr++ = *scanptr;
            }
        }
        *duplicatesptr = 0;
        for (char *scanptr = duplicate_items; *scanptr; scanptr++) {
            if (strchr(input_buffers[2], *scanptr)) {
                totalscore += priority(*scanptr);
                break;
            }
        }
    }
    printf("%d\n", totalscore);
    return 0;
}
