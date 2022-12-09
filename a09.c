#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define LINE_LENGTH 16
#define KNOTS 10

// WATCH OUT; adjust order of variables to achieve correct output
union position {
    struct {
        uint32_t y;
        uint32_t x;
    };
    uint64_t value;
};

// Using tree set preemptively in case Part 2 demands ordering
struct tree_node {
    struct tree_node *left;
    struct tree_node *right;
    union position pos;
};

void move_head(union position *head, char dir) {
    switch (dir) {
        case 'U':
            (head->y)--;
            break;
        case 'D':
            (head->y)++;
            break;
        case 'L':
            (head->x)--;
            break;
        case 'R':
            (head->x)++;
    }
}

void move_tail(union position *head, union position *tail) {
    union position difference;
    difference.x = head->x - tail->x;
    difference.y = head->y - tail->y;
    switch (difference.value) {
        case 0xFFFFFFFE00000000:
            (tail->x)--;
            break;
        case 0x0000000200000000:
            (tail->x)++;
            break;
        case 0x00000000FFFFFFFE:
            (tail->y)--;
            break;
        case 0x0000000000000002:
            (tail->y)++;
            break;
        case 0xFFFFFFFEFFFFFFFF:
        case 0xFFFFFFFFFFFFFFFE:
        case 0xFFFFFFFEFFFFFFFE:
            (tail->x)--;
            (tail->y)--;
            break;
        case 0xFFFFFFFE00000001:
        case 0xFFFFFFFF00000002:
        case 0xFFFFFFFE00000002:
            (tail->x)--;
            (tail->y)++;
            break;
        case 0x0000000200000001:
        case 0x0000000100000002:
        case 0x0000000200000002:
            (tail->x)++;
            (tail->y)++;
            break;
        case 0x00000001FFFFFFFE:
        case 0x00000002FFFFFFFF:
        case 0x00000002FFFFFFFE:
            (tail->x)++;
            (tail->y)--;
    }
}

struct tree_node *allocate_tree_node(uint64_t value) {
    struct tree_node *new = malloc(sizeof(*new));
    if (!new) {
        puts("Error allocating memory");
        exit(1);
    }
    new->left = NULL;
    new->right = NULL;
    new->pos.value = value;
    return new;
}

unsigned int free_tree(struct tree_node *tree) {
    if (!tree) {
        return 0;
    }
    unsigned int total = free_tree(tree->left) + free_tree(tree->right) + 1;
    free(tree);
    return total;
}

void tree_add(struct tree_node *root, union position *p) {
    if (root->pos.value == p->value) {
        return;
    }
    if (root->pos.value > p->value) {
        if (!root->left) {
            root->left = allocate_tree_node(p->value);
        } else {
            tree_add(root->left, p);
        }
    } else {
        if (!root->right) {
            root->right = allocate_tree_node(p->value);
        } else {
            tree_add(root->right, p);
        }
    }
}

int main() {
    static union position knots[KNOTS];
    char line_buffer[LINE_LENGTH];
    struct tree_node *position_tree = allocate_tree_node(0);
    while (fgets(line_buffer, LINE_LENGTH, stdin)) {
        char direction;
        unsigned short r;
        sscanf(line_buffer, "%c %hu\n", &direction, &r);
        while (r--) {
            move_head(knots, direction);
            for (int tail = 1; tail < KNOTS; tail++) {
                move_tail(knots + tail - 1, knots + tail);
            }
            tree_add(position_tree, knots + KNOTS - 1);
        }
    }
    printf("%u\n", free_tree(position_tree));
    return 0;
}
