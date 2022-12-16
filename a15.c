#include <stdio.h>
#include <stdlib.h>

#define LINE_LENGTH 256
#define BEACON_STRING "Sensor at x=%ld, y=%ld: closest beacon is at x=%ld, y=%ld\n"
#define D_RANGE 4000000LL

struct scanner_node {
    struct scanner_node *next;
    long x;
    long y;
    long tdist;
};

void free_scanner_list(struct scanner_node *scanner_list) {
    if (!scanner_list) {
        return;
    }
    free_scanner_list(scanner_list->next);
    free(scanner_list);
}

int main() {
    char buffer[LINE_LENGTH];
    struct scanner_node *scanner_list = NULL;
    while (fgets(buffer, LINE_LENGTH, stdin)) {
        struct scanner_node *new = malloc(sizeof(*new));
        if (!new) {
            puts("Error allocating memory");
            return 1;
        }
        new->next = scanner_list;
        scanner_list = new;
        long bx;
        long by;
        sscanf(buffer, BEACON_STRING, &new->x, &new->y, &bx, &by);
        new->tdist = labs(new->x - bx) + labs(new->y - by);
    }
    for (register long y = 0; y <= D_RANGE; y++) {
        register long x = 0;
        while (x <= D_RANGE) {
            for (struct scanner_node *s = scanner_list; s; s = s->next) {
                register long sxdist = s->tdist - labs(s->y - y);
                if (labs(s->x - x) <= sxdist) {
                    x = s->x + sxdist + 1;
                    goto end_checks;
                }
            }
            free_scanner_list(scanner_list);
            printf("%lld\n", D_RANGE * x + y);
            return 0;
        end_checks:;
        }
    }
    return 0;
}
