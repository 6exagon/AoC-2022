#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

#define KEY 811589153
#define CHECK_PTR(p) if (!p) { puts("Error allocating memory"); return 1; }

// Singly-linked list node, keeps track of which exact number to move around
struct lln {
    struct lln *next;
    struct dlln *number;
};

// Doubly-linked list node, holds the number itself
struct dlln {
    struct dlln *next;
    struct dlln *prev;
    int64_t value;
};

struct dlln *seek_zero(struct dlln *start) {
    while (start->value) {
        start = start->next;
    }
    return start;
}

// This is necessary because numbers can occur more than once, a very frustrating realization
struct dlln *seek_order_node(struct dlln *start, struct dlln *seeking) {
    while (start->value) {
        start = start->next;
    }
    return start;
}

struct dlln *forward(struct dlln *start, int64_t places, int length) {
    places %= length;
    if (places > 0) {
        while (places--) {
            start = start->next;
        }
    } else if (places < 0 ) {
        while (places++) {
            start = start->prev;
        }
    }
    return start;
}

void free_lists(struct lln *orderlist) {
    while (orderlist) {
        struct lln *tempnext = orderlist->next;
        free(orderlist->number);
        free(orderlist);
        orderlist = tempnext;
    }
}

int main() {
    int64_t newvalue;
    scanf("%" PRId64 "\n", &newvalue);
    struct lln *orderlist = malloc(sizeof(*orderlist));
    struct lln *orderlistend = orderlist;
    struct dlln *shufflelist = malloc(sizeof(*shufflelist));
    CHECK_PTR(orderlist)
    CHECK_PTR(shufflelist)
    orderlistend->number = shufflelist;
    shufflelist->next = shufflelist;
    shufflelist->prev = shufflelist;
    shufflelist->value = newvalue * KEY;
    int length = 0;
    while (scanf("%" PRId64 "\n", &newvalue) != EOF) {
        orderlistend->next = malloc(sizeof(*orderlistend->next));
        struct dlln *newshufflenode = malloc(sizeof(*newshufflenode));
        CHECK_PTR(orderlistend->next)
        CHECK_PTR(newshufflenode)
        orderlistend = orderlistend->next;
        orderlistend->number = newshufflenode;
        newshufflenode->next = shufflelist->next;
        newshufflenode->prev = shufflelist;
        newshufflenode->value = newvalue * KEY;
        shufflelist->next = newshufflenode;
        shufflelist = newshufflenode;
        shufflelist->next->prev = shufflelist;
        length++;
    }
    orderlistend->next = NULL;
    for (short x = 0; x < 10; x++) {
        struct lln *orderscan = orderlist;
        while (orderscan) {
            struct dlln *holding_node = orderscan->number;
            holding_node->prev->next = holding_node->next;
            holding_node->next->prev = holding_node->prev;
            shufflelist = forward(holding_node->prev, holding_node->value, length);
            holding_node->prev = shufflelist;
            holding_node->next = shufflelist->next;
            shufflelist->next = holding_node;
            holding_node->next->prev = holding_node;
            orderscan = orderscan->next;
        }
    }
    int64_t sum = 0;
    shufflelist = seek_zero(shufflelist);
    length++;
    for (short x = 0; x < 3; x++) {
        shufflelist = forward(shufflelist, 1000, length);
        sum += shufflelist->value;
    }
    printf("%" PRId64 "\n", sum);
    free_lists(orderlist);
    return 0;
}
