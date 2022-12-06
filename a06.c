#include <stdio.h>
#include <string.h>

#define P_LENGTH 14

unsigned char no_unique(char *buffer) {
    for (char *cc = buffer + P_LENGTH - 1; cc-- > buffer;) {
        if (strchr(cc + 1, *cc)) {
            return 0;
        }
    }
    return 1;
}

int main() {
    static char packet_buffer[P_LENGTH + 1];
    fgets(packet_buffer, P_LENGTH, stdin);
    unsigned int chars_read = P_LENGTH - 1;
    while (1) {
        packet_buffer[chars_read++ % P_LENGTH] = getchar();
        if (no_unique(packet_buffer)) {
            printf("%d\n", chars_read);
            return 0;
        }
    }
}
