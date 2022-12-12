#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>

#define MAP_LENGTH 200

const int8_t step_options[4][2] = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};
char map[MAP_LENGTH][MAP_LENGTH];
uint16_t stepmap[MAP_LENGTH][MAP_LENGTH];

void steppath(uint8_t row, uint8_t column) {
    register char current_level = map[row][column];
    register uint16_t current_steps = stepmap[row][column];
    for (uint8_t opt = 0; opt < 4; opt++) {
        register uint8_t newrow = row + step_options[opt][0];
        register uint8_t newcolumn = column + step_options[opt][1];
        register char new_level = map[newrow][newcolumn];
        if (!new_level || new_level == '\n' || new_level > current_level + 1) {
            continue;
        }
        if (stepmap[newrow][newcolumn] > stepmap[row][column] + 1) {
            stepmap[newrow][newcolumn] = stepmap[row][column] + 1;
            steppath(newrow, newcolumn);
        }
    }
}

int main() {
    uint8_t end_row;
    uint8_t end_column;
    uint8_t row;
    for (row = 1; fgets(map[row] + 1, MAP_LENGTH, stdin); row++) {
        char *special = strchr(map[row] + 1, 'S');
        if (special) {
            *special = 'a';
        }
        special = strchr(map[row] + 1, 'E');
        if (special) {
            end_row = row;
            end_column = special - map[row];
            *special = 'z';
        }
    }
    uint16_t min_steps = UINT16_MAX;
    uint8_t width = strlen(map[1] + 1);
    while (--row) {
        uint8_t column = width;
        while (--column) {
            if (map[row][column] == 'a') {
                memset(stepmap, UINT8_MAX, MAP_LENGTH * MAP_LENGTH * sizeof(**stepmap));
                stepmap[row][column] = 0;
                steppath(row, column);
                uint16_t end_steps = stepmap[end_row][end_column];
                min_steps = (min_steps < end_steps) ? min_steps : end_steps;
            }
        }
    }
    printf("%" PRIu16 "\n", min_steps);
    return 0;
}
