#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define LINE_LENGTH 12000
#define BOARD_WIDTH 9
#define NPIECES 5
#define ROCKS 1000000000000LL

struct vec2d {
    uint8_t x;
    uint8_t y;
};

// Assumes little-endian system for speed, unfortunately
// Uses out-of-bounds indexes
union board_row {
    uint64_t value;
    uint8_t row[8];
};

const struct vec2d pieces[NPIECES][5] = {
    {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {0, 0}},
    {{1, 0}, {0, 1}, {1, 1}, {2, 1}, {1, 2}},
    {{0, 0}, {1, 0}, {2, 0}, {2, 1}, {2, 2}},
    {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 0}},
    {{0, 0}, {1, 0}, {0, 1}, {1, 1}, {0, 0}}
};

uint8_t can_place_piece(
    const struct vec2d piece[5], union board_row *board, uint8_t x, uint16_t y) {
    for (uint8_t b = 0; b < 5; b++) {
        if (board[(uint16_t) (y + piece[b].y)].row[x + piece[b].x]) {
            return 0;
        }
    }
    return 1;
}

static inline void place_piece(
    const struct vec2d piece[5], union board_row *board, uint8_t x, uint16_t y) {
    for (uint8_t b = 0; b < 5; b++) {
        board[(uint16_t) (y + piece[b].y)].row[x + piece[b].x] = '#';
    }
}

static inline void clear_above(union board_row *board, uint16_t y) {
    for (uint8_t h = 0; h < 4; h++) {
        board[y++].value = 1;
    }
}

int main() {
    char wind[LINE_LENGTH];
    fgets(wind, LINE_LENGTH, stdin);
    char *jetptr = wind;
    union board_row *board = malloc((UINT16_MAX + 2) * sizeof(*board));
    if (!board) {
        puts("Error allocating memory");
        return 1;
    }
    board[UINT16_MAX + 1].value = 1;
    board[0].value = 0xFFFFFFFFFFFFFFFF;
    clear_above(board, 1);
    uint64_t floor = 1;
    // Sadly, this solution uses brute force
    // I was unable to implement a faster method by the time the brute force finished
    for (uint64_t rock = 0; rock < ROCKS; rock++) {
        clear_above(board, floor + 3);
        uint8_t x = 3;
        uint16_t y = floor + 3;
        const struct vec2d *piece = pieces[rock % NPIECES];
        do {
            uint8_t newx = x + ((*jetptr++ == '<') ? -1 : 1);
            if (*jetptr == '\n') {
                jetptr = wind;
            }
            if (can_place_piece(piece, board, newx, y)) {
                x = newx;
            }
            y--;
        } while (can_place_piece(piece, board, x, y));
        place_piece(piece, board, x, ++y);
        while (board[(uint16_t) floor].value != 1) {
            floor++;
        }
    }
    free(board);
    printf("%llu\n", --floor);
    return 0;
}
