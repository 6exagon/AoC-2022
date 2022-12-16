#include <stdio.h>
#include <string.h>

#define CHUNK_WIDTH 1000
#define CHUNK_HEIGHT 256

int main() {
    static char board[CHUNK_HEIGHT][CHUNK_WIDTH];
    short startx;
    short starty;
    short maxy = 0;
    while (scanf("%hd,%hd", &startx, &starty) != EOF) {
        maxy = (starty > maxy) ? starty : maxy;
        board[starty][startx] = '#';
        short endx;
        short endy;
        while (getchar() != '\n') {
            scanf("-> %hd,%hd", &endx, &endy);
            maxy = (endy > maxy) ? endy : maxy;
            while (endx != startx || endy != starty) {
                startx += (endx > startx);
                startx -= (endx < startx);
                starty += (endy > starty);
                starty -= (endy < starty);
                board[starty][startx] = '#';
            }
        }
    }
    memset(board[maxy + 2], 'F', CHUNK_WIDTH);
    unsigned int grains = 0;
    while (1) {
        grains++;
        short sandx = 500;
        short sandy = 0;
        while (1) {
            if (!board[sandy + 1][sandx]) {
                sandy++;
            } else if (!board[sandy + 1][sandx - 1]) {
                sandy++;
                sandx--;
            } else if (!board[sandy + 1][sandx + 1]) {
                sandy++;
                sandx++;
            } else {
                board[sandy][sandx] = 'o';
                if (!sandy) {
                    printf("%u\n", grains);
                    return 0;
                }
                break;
            }
        }
    }
}
