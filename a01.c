#include <stdio.h>
#include <stdlib.h>

#define LINE_SIZE 32

int main() {
    char buffer[LINE_SIZE];
    long i = 0;
    long maxes[3] = {0};
    while (fgets(buffer, LINE_SIZE, stdin)) {
        if (*buffer == '\n') {
            for (int x = 0; x < 3; x++) {
                if (i > maxes[x]) {
                    int temp = maxes[x];
                    maxes[x] = i;
                    i = temp;
                }
            }
            i = 0;
        } else {
            i += atol(buffer);
        }
    }
    printf("%ld\n", maxes[0] + maxes[1] + maxes[2]);
    return 0;
}
