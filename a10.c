#include <stdio.h>

#define LINE_LENGTH 16
#define CRT_WIDTH 40

int main() {
    char buffer[LINE_LENGTH];
    char screen_row[CRT_WIDTH + 1] = {0};
    unsigned short cycle = 0;
    int xregister = 1;
    while (fgets(buffer, LINE_LENGTH, stdin)) {
        screen_row[cycle] = (cycle > xregister + 1 || cycle < xregister - 1) ? ' ' : '@';
        if (++cycle == CRT_WIDTH) {
            cycle = 0;
            puts(screen_row);
        }
        short addvalue;
        if (sscanf(buffer, "addx %hd\n", &addvalue)) {
            screen_row[cycle] = (cycle > xregister + 1 || cycle < xregister - 1) ? ' ' : '@';
            if (++cycle == CRT_WIDTH) {
                cycle = 0;
                puts(screen_row);
            }
            xregister += addvalue;
        }
    }
    return 0;
}
