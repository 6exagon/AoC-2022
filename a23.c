#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// This may be slightly inefficient, but it's more readable
struct elf {
    short x;
    short y;
    short nextx;
    short nexty;
};

struct elflistnode {
    struct elf *elf;
    struct elflistnode *next;
};

void choosedirection(struct elflistnode *elflist, struct elf *elf, short round) {
    char openspaces[3][3] = {0};
    char filled = 0;
    short ex = elf->x;
    short ey = elf->y;
    while (elflist) {
        short scanex = elflist->elf->x;
        short scaney = elflist->elf->y;
        if (abs(scanex - ex) < 2 && abs(scaney - ey) < 2) {
            openspaces[scaney - ey + 1][scanex - ex + 1] = 1;
            filled++;
        }
        elflist = elflist->next;
    }
    if (filled < 2) {
        return;
    }
    elf->nextx = ex;
    elf->nexty = ey;
    for (short dircheck = 0; dircheck < 4; dircheck++) {
        switch ((dircheck + round) & 3) {
            case 0:
                if (!openspaces[0][0] && !openspaces[0][1] && !openspaces[0][2]) {
                    elf->nexty--;
                    return;
                }
                break;
            case 1:
                if (!openspaces[2][0] && !openspaces[2][1] && !openspaces[2][2]) {
                    elf->nexty++;
                    return;
                }
                break;
            case 2:
                if (!openspaces[0][0] && !openspaces[1][0] && !openspaces[2][0]) {
                    elf->nextx--;
                    return;
                }
                break;
            case 3:
                if (!openspaces[0][2] && !openspaces[1][2] && !openspaces[2][2]) {
                    elf->nextx++;
                    return;
                }
        }
    }
}

int simulate(struct elflistnode *elflist) {
    int round = 0;
    for (char moved = 1; moved; round++) {
        for (struct elflistnode *c_elf_node = elflist; c_elf_node; c_elf_node = c_elf_node->next) {
            choosedirection(elflist, c_elf_node->elf, round);
        }
        for (struct elflistnode *c_elf_node = elflist; c_elf_node; c_elf_node = c_elf_node->next) {
            short nextex = c_elf_node->elf->nextx;
            short nextey = c_elf_node->elf->nexty;
            for (struct elflistnode *s = c_elf_node->next; s; s = s->next) {
                if (s->elf->nextx == nextex && s->elf->nexty == nextey) {
                    c_elf_node->elf->nextx = c_elf_node->elf->x;
                    c_elf_node->elf->nexty = c_elf_node->elf->y;
                    s->elf->nextx = s->elf->x;
                    s->elf->nexty = s->elf->y;
                }
            }
        }
        moved = 0;
        for (struct elflistnode *cen = elflist; cen; cen = cen->next) {
            if (!moved && (cen->elf->nextx != cen->elf->x || cen->elf->nexty != cen->elf->y)) {
                moved = 1;
            }
            cen->elf->x = cen->elf->nextx;
            cen->elf->y = cen->elf->nexty;
        }
    }
    return round;
}

void freeelist(struct elflistnode *elflist) {
    while (elflist) {
        struct elflistnode *next = elflist->next;
        free(elflist->elf);
        free(elflist);
        elflist = next;
    }
}

int main() {
    short x = 0;
    short y = 0;
    char nextchar;
    struct elflistnode *elflist = NULL;
    while ((nextchar = getchar()) != EOF) {
        struct elflistnode *newllnode;
        struct elf *newelf;
        switch (nextchar) {
            case '\n':
                y++;
                x = 0;
                break;
            case '#':
                newllnode = malloc(sizeof(*newllnode));
                newelf = malloc(sizeof(*newelf));
                if (!newllnode || !newelf) {
                    puts("Error allocating memory");
                    return 1;
                }
                newelf->x = x;
                newelf->y = y;
                newllnode->elf = newelf;
                newllnode->next = elflist;
                elflist = newllnode;
            case '.':
                x++;
        }
    }
    printf("%d\n", simulate(elflist));
    freeelist(elflist);
    return 0;
}
