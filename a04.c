#include <stdio.h>

#define LINE_SIZE 32

int main() {
    char buffer[LINE_SIZE];
    unsigned int opairs = 0;
    while (fgets(buffer, LINE_SIZE, stdin)) {
        unsigned int a1, a2, b1, b2;
        sscanf(buffer, "%u-%u,%u-%u", &a1, &a2, &b1, &b2);
        opairs += (a2 >= b1 && b2 >= a1);
    }
    printf("%u\n", opairs);
    return 0;
}
