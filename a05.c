#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_SIZE 256
#define STACK_LIMIT 256
#define CHECK_POINTER(pointer) if (!pointer) {puts("Error allocating memory"); return 1;}

char stacks_push(char **stacks, unsigned int s, char crate) {
    return *stacks[s]++ = crate;
}

char stacks_pop(char **stacks, unsigned int s) {
    return *--stacks[s];
}

char **get_crate_stacks(unsigned int n) {
    char **stacks = malloc(sizeof(*stacks) * n);
    if (!stacks) {
        return NULL;
    }
    while (n--) {
        stacks[n] = malloc(STACK_LIMIT);
        if (!stacks[n]) {
            return NULL;
        }
        // This is so stacks can stop before underflowing when popping all values
        stacks_push(stacks, n, 0);
    }
    return stacks;
}

void free_crate_stacks(char **stacks, unsigned int n) {
    while (n--) {
        if (*stacks[n]) {
            while (stacks_pop(stacks, n));
        }
        free(stacks[n]);
    }
    free(stacks);
}

int main() {
    char buffer[LINE_SIZE];
    
    fgets(buffer, LINE_SIZE, stdin);
    unsigned int num_stacks = strlen(buffer) / 4;
    char **temp_stacks = get_crate_stacks(num_stacks);
    CHECK_POINTER(temp_stacks)
    while (buffer[1] != '1') {
        for (unsigned int x = 0; x < num_stacks; x++) {
            if (buffer[x * 4 + 1] != ' ') {
                stacks_push(temp_stacks, x, buffer[x * 4 + 1]);
            }
        }
        fgets(buffer, LINE_SIZE, stdin);
    }
    
    char **crate_stacks = get_crate_stacks(num_stacks);
    CHECK_POINTER(crate_stacks)
    for (unsigned int x = 0; x < num_stacks; x++) {
        while (stacks_push(crate_stacks, x, stacks_pop(temp_stacks, x)));
        // Pop safety zero that was pulled from the very bottom of the temporary stack
        stacks_pop(crate_stacks, x);
    }
    free_crate_stacks(temp_stacks, num_stacks);
    
    // Burn newline
    getchar();
    while (fgets(buffer, LINE_SIZE, stdin)) {
        unsigned int moves, from, to;
        sscanf(buffer, "move %u from %u to %u", &moves, &from, &to);
        // Zero-based stack indexing
        memcpy(crate_stacks[--to], crate_stacks[--from] -= moves, moves);
        crate_stacks[to] += moves;
    }
    for (unsigned int x = 0; x < num_stacks; x++) {
        putchar(crate_stacks[x][-1]);
    }
    putchar('\n');
    free_crate_stacks(crate_stacks, num_stacks);
    return 0;
}
