#include <stdio.h>

#define LINE_SIZE 5

const int throwpts[3][3] = {
    {3, 1, 2},
    {1, 2, 3},
    {2, 3, 1}
};

int main() {
    char buffer[LINE_SIZE];
    int totalscore = 0;
    while (fgets(buffer, LINE_SIZE, stdin)) {
        totalscore += (buffer[2] - 'X') * 3 + throwpts[buffer[0] - 'A'][buffer[2] - 'X'];
    }
    printf("%d\n", totalscore);
    return 0;
}
