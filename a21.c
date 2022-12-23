#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define MAX_LINES 5000
#define LINE_LENGTH 32

static inline int lineof(char *name, char data[MAX_LINES][LINE_LENGTH]) {
    int line = -1;
    while (strcmp(name, data[++line]));
    return line;
}

int64_t solve(char *name, char data[MAX_LINES][LINE_LENGTH]) {
    int line = lineof(name, data);
    int64_t value;
    if (sscanf(&data[line][6], "%lld", &value)) {
        return value;
    }
    int64_t a = solve(&data[line][6], data);
    int64_t b = solve(&data[line][13], data);
    if (a == INT64_MIN || b == INT64_MIN) {
        return INT64_MIN;
    }
    switch (data[line][11]) {
        case '+':
            return a + b;
        case '-':
            return a - b;
        case '*':
            return a * b;
        case '/':
            return a / b;
    }
    // For debugging purposes
    return -1;
}

int64_t algebra(char *name, int64_t equalto, char data[MAX_LINES][LINE_LENGTH]) {
    if (!strcmp(name, "humn")) {
        return equalto;
    }
    int line = lineof(name, data);
    char *left = &data[line][6];
    char *right = &data[line][13];
    int64_t leftsolve = solve(left, data);
    int64_t rightsolve = solve(right, data);
    if (leftsolve == INT64_MIN) {
        switch (data[line][11]) {
            case '+':
                return algebra(left, equalto - rightsolve, data);
            case '-':
                return algebra(left, equalto + rightsolve, data);
            case '*':
                return algebra(left, equalto / rightsolve, data);
            case '/':
                return algebra(left, equalto * rightsolve, data);
        }
    } else {
        switch (data[line][11]) {
            case '+':
                return algebra(right, equalto - leftsolve, data);
            case '-':
                return algebra(right, -(equalto - leftsolve), data);
            case '*':
                return algebra(right, equalto / leftsolve, data);
            case '/':
                return algebra(right, 1 / (equalto / leftsolve), data);
        }
    }
    // For debugging purposes
    return -1;
}

int main() {
    static char data[MAX_LINES][LINE_LENGTH];
    for (int line = 0; fgets(data[line], LINE_LENGTH, stdin); line++) {
        data[line][4] = 0;
        data[line][10] = 0;
        data[line][17] = 0;
    }
    int rootline = lineof("root", data);
    sprintf(&data[lineof("humn", data)][6], "%lld", INT64_MIN);
    char *left = &data[rootline][6];
    char *right = &data[rootline][13];
    int64_t leftsolve = solve(left, data);
    int64_t rightsolve = solve(right, data);
    if (leftsolve == INT64_MIN) {
        printf("%lld\n", algebra(left, rightsolve, data));
    } else {
        printf("%lld\n", algebra(right, leftsolve, data));
    }
    return 0;
}
