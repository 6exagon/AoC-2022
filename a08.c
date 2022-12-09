#include <stdio.h>
#include <string.h>

#define MAP_LENGTH 256

int main() {
    char map[MAP_LENGTH][MAP_LENGTH] = {0};
    unsigned short row = 0;
    while (fgets(map[row++], MAP_LENGTH, stdin));
    unsigned int max_treescore = 0;
    unsigned short column;
    short check;
    char height;
    row--;
    while (row--) {
        for (column = 0; map[0][column] != '\n'; column++) {
            // This is terrible
            unsigned int treescore = 1;
            unsigned int dirscore = 0;
            for (check = row + 1; map[check][column]; check++) {
                dirscore++;
                if (map[check][column] >= map[row][column]) {
                    break;
                }
            }
            treescore *= dirscore;
            dirscore = 0;
            for (check = row - 1; check > -1; check--) {
                dirscore++;
                if (map[check][column] >= map[row][column]) {
                    break;
                }
            }
            treescore *= dirscore;
            dirscore = 0;
            for (check = column + 1; map[row][check] != '\n'; check++) {
                dirscore++;
                if (map[row][check] >= map[row][column]) {
                    break;
                }
            }
            treescore *= dirscore;
            dirscore = 0;
            for (check = column - 1; check > -1; check--) {
                dirscore++;
                if (map[row][check] >= map[row][column]) {
                    break;
                }
            }
            treescore *= dirscore;
            max_treescore = (max_treescore > treescore) ? max_treescore : treescore;
        }
    }
    printf("%u\n", max_treescore);
    return 0;
}
