#include <stdio.h>
#include <stdint.h>

// Maybe caching, indexing, etc. will be easier with 256
#define MAX_WIDTH 256
#define MAX_HEIGHT 40
#define NFLAG 1
#define SFLAG 2
#define WFLAG 4
#define EFLAG 8

uint8_t board[MAX_HEIGHT][MAX_WIDTH];
uint8_t nextposmem[MAX_HEIGHT + 1][MAX_WIDTH];
uint8_t width, height;
int16_t target_x, target_y;
const int8_t disps[5][2] = {{1, 0}, {0, 1}, {0, 0}, {0, -1}, {-1, 0}};

static inline void advance_blizzards() {
    for (uint8_t y = 0; y < height; y++) {
        for (uint8_t x = 0; x < width; x++) {
            if (board[y][x] & NFLAG) {
                uint8_t py = (y) ? y - 1 : height - 1;
                board[py][x] |= NFLAG << 4;
            }
            if (board[y][x] & SFLAG) {
                uint8_t py = (y + 1) % height;
                board[py][x] |= SFLAG << 4;
            }
            if (board[y][x] & WFLAG) {
                uint8_t px = (x) ? x - 1 : width - 1;
                board[y][px] |= WFLAG << 4;
            }
            if (board[y][x] & EFLAG) {
                uint8_t px = (x + 1) % width;
                board[y][px] |= EFLAG << 4;
            }
        }
    }
    for (uint8_t y = 0; y < height; y++) {
        for (uint8_t x = 0; x < width; x++) {
            board[y][x] >>= 4;
        }
    }
}

static inline void shift_nextpos() {
    for (uint8_t y = 0; y < height + 2; y++) {
        for (uint8_t x = 0; x < width; x++) {
            nextposmem[y][x] >>= 1;
        }
    }
}

// Implicit promotions to unsigned types useful for less logic
static inline char is_valid_position(uint8_t ex, uint8_t ey) {
    if ((ex == width - 1 && ey == height) || (ex == 0 && ey == UINT8_MAX)) {
        return 1;
    }
    if (ex >= width || ey >= height) {
        return 0;
    }
    return 1;
}

static inline char explore(uint8_t nextpos[MAX_HEIGHT][MAX_WIDTH], uint8_t ex, int16_t ey) {
    if (ex == target_x && ey == target_y) {
        return 1;
    }
    // End if we're standing on a blizzard
    if (ey > -1 && board[ey][ex]) {
        return 0;
    }
    for (char d = 0; d < 5; d++) {
        register int16_t nex = ex + disps[d][0];
        register int16_t ney = ey + disps[d][1];
        if (is_valid_position(nex, ney)) {
            nextpos[ney][nex] |= 2;
        }
    }
    return 0;
}

uint16_t explore_loop() {
    uint16_t depth = 0;
    for (char round = 0; round < 3; round++) {
        shift_nextpos();
        if (round % 2) {
            nextposmem[height + 1][width - 1] = 1;
            target_y = -1;
            target_x = 0;
        } else {
            nextposmem[0][0] = 1;
            target_y = height;
            target_x = width - 1;
        }
        while (++depth) {
            for (uint8_t y = 0; y < height + 2; y++) {
                for (uint8_t x = 0; x < width; x++) {
                    if (nextposmem[y][x] & 1) {
                        if (explore(&nextposmem[1], x, y - 1)) {
                            shift_nextpos();
                            advance_blizzards();
                            goto end_round;
                        }
                    }
                }
            }
            shift_nextpos();
            advance_blizzards();
        }
    end_round:;
    }
    return --depth;
}

int main() {
    uint8_t x = -1;
    uint8_t y = -1;
    while (1) {
        switch (getchar()) {
            case EOF:
                height = y - 1;
                printf("%d\n", explore_loop());
                return 0;
            case '\n':
                y++;
                width = x - 1;
                x = -1;
                break;
            case '^':
                board[y][x++] = NFLAG;
                break;
            case 'v':
                board[y][x++] = SFLAG;
                break;
            case '<':
                board[y][x++] = WFLAG;
                break;
            case '>':
                board[y][x++] = EFLAG;
                break;
            default:
                x++;
        }
    }
}
