#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define LINE_LENGTH 2048
#define LINE_LIMIT 500
#define SINGLE_LIST_FLAG 64
#define CHECK_PTR(p) if (!p) { puts("Error allocating memory"); return 1; }
#define D_A "[[2]]"
#define D_B "[[6]]"

void next(char **p) {
    if (isdigit(**p)) {
        do {
            (*p)++;
        } while (isdigit(**p));
    } else {
        (*p)++;
    }
    if (**p == ',') {
        (*p)++;
    }
}

int compare(const void *a, const void *b) {
    char leftbuffer[LINE_LENGTH];
    char rightbuffer[LINE_LENGTH];
    strcpy(leftbuffer, *(const char **)a);
    strcpy(rightbuffer, *(const char **)b);
    char *lefthead = leftbuffer;
    char *righthead = rightbuffer;
    while (1) {
        if (*lefthead == ']' && *righthead == ']') {
            next(&lefthead);
            next(&righthead);
        } else if (*lefthead == ']') {
            return -1;
        } else if (*righthead == ']') {
            return 1;
        } else if (*lefthead == '[' && *righthead == '[') {
            next(&lefthead);
            next(&righthead);
        } else if (*lefthead == '[') {
            next(&lefthead);
            *righthead |= SINGLE_LIST_FLAG;
        } else if (*righthead == '[') {
            *lefthead |= SINGLE_LIST_FLAG;
            next(&righthead);
        } else {
            short leftint;
            short rightint;
            char leftsl = *lefthead & SINGLE_LIST_FLAG;
            char rightsl = *righthead & SINGLE_LIST_FLAG;
            *lefthead &= SINGLE_LIST_FLAG - 1;
            *righthead &= SINGLE_LIST_FLAG - 1;
            sscanf(lefthead, "%hd", &leftint);
            sscanf(righthead, "%hd", &rightint);
            if (leftint != rightint) {
                return (leftint - rightint);
            }
            next(&lefthead);
            if (leftsl) {
                *--lefthead = ']';
            }
            next(&righthead);
            if (rightsl) {
                *--righthead = ']';
            }
        }
    }
}

int main() {
    char *lines[LINE_LIMIT];
    short line = 0;
    while (1) {
        char *newstring = malloc(LINE_LENGTH);
        CHECK_PTR(newstring)
        lines[line] = newstring;
        if (!fgets(newstring, LINE_LENGTH, stdin)) {
            strcpy(newstring, D_A);
            newstring = malloc(LINE_LENGTH);
            CHECK_PTR(newstring)
            lines[++line] = newstring;
            strcpy(newstring, D_B);
            break;
        }
        if (*lines[line] == '\n') {
            free(newstring);
        } else {
            line++;
        }
    }
    qsort(lines, ++line, sizeof(*lines), compare);
    unsigned int d_a = 0;
    unsigned int d_b = 0;
    for (short x = 0; !d_a || !d_b; x++) {
        if (!strcmp(lines[x], D_A)) {
            d_a = x + 1;
        }
        if (!strcmp(lines[x], D_B)) {
            d_b = x + 1;
        }
    }
    while (line--) {
        free(lines[line]);
    }
    printf("%u\n", d_a * d_b);
    return 0;
}
