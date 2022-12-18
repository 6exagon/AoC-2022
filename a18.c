#include <stdio.h>

#define SIZE 24

unsigned int scan(char volume[SIZE][SIZE][SIZE], short x, short y, short z) {
    if (x < 0 || y < 0 || z < 0 || x == SIZE || y == SIZE || z == SIZE || volume[x][y][z] == 2) {
        return 0;
    }
    if (volume[x][y][z]) {
        return 1;
    }
    volume[x][y][z] = 2;
    return scan(volume, x + 1, y, z) +
        scan(volume, x - 1, y, z) +
        scan(volume, x, y + 1, z) +
        scan(volume, x, y - 1, z) +
        scan(volume, x, y, z + 1) +
        scan(volume, x, y, z - 1);
}

int main() {
    static char volume[SIZE][SIZE][SIZE];
    short x, y, z;
    while (scanf("%hd,%hd,%hd\n", &x, &y, &z) != EOF) {
        volume[++x][++y][++z] = 1;
    }
    printf("%u\n", scan(volume, 0, 0, 0));
}
