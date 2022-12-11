#include <stdio.h>
#include <stdlib.h>

#define LINE_LENGTH 256
#define MAX_MONKEYS 32
#define CHECK_PTR(p) if (!p) { puts("Error allocating memory"); exit(1); }

struct itemqueuenode {
    struct itemqueuenode *next;
    long long item;
};

struct monkey {
    struct itemqueuenode *front;
    struct itemqueuenode *back;
    unsigned long long inspection_count;
    short operand;
    short testdivisor;
    short truemonkey;
    short falsemonkey;
    char operator;
};

unsigned long gcd(unsigned long a, unsigned long b) {
    if (!b) {
        return a;
    }
    return gcd(b, a % b);
}

void white_line(char *line) {
    while (*++line != '\n') {
        if (*line < '0' || *line > '9') {
            *line = ' ';
        }
    }
}

void monkey_queue_add(struct monkey *m, long long item) {
    if (!m->back) {
        m->back = malloc(sizeof(*m->back));
        CHECK_PTR(m->back)
        m->front = m->back;
    } else {
        m->back->next = malloc(sizeof(*m->back->next));
        CHECK_PTR(m->back->next)
        m->back = m->back->next;
    }
    m->back->next = NULL;
    m->back->item = item;
}

long long monkey_queue_remove(struct monkey *m) {
    if (!m->back) {
        return 0;
    }
    long long popped = m->front->item;
    struct itemqueuenode *new_front = m->front->next;
    free(m->front);
    m->front = new_front;
    if (!new_front) {
        m->back = NULL;
    }
    return popped;
}

struct monkey *read_new_monkey() {
    char buffer[LINE_LENGTH];
    if (!fgets(buffer, LINE_LENGTH, stdin)) {
        return NULL;
    }
    struct monkey *new_monkey = malloc(sizeof(*new_monkey));
    CHECK_PTR(new_monkey)
    new_monkey->back = NULL;
    fgets(buffer, LINE_LENGTH, stdin);
    white_line(buffer);
    char *num_reader = buffer;
    while (*num_reader != '\n') {
        monkey_queue_add(new_monkey, strtol(num_reader, &num_reader, 10));
    }
    fgets(buffer, LINE_LENGTH, stdin);
    if (sscanf(
        buffer,
        "  Operation: new = old %c %hd\n",
        &new_monkey->operator,
        &new_monkey->operand) == 1) {
        // "old" is the operand, so operation should be square
        new_monkey->operator = '$';
    }
    fgets(buffer, LINE_LENGTH, stdin);
    sscanf(buffer, "  Test: divisible by %hd\n", &new_monkey->testdivisor);
    fgets(buffer, LINE_LENGTH, stdin);
    sscanf(buffer, "    If true: throw to monkey %hd\n", &new_monkey->truemonkey);
    fgets(buffer, LINE_LENGTH, stdin);
    sscanf(buffer, "    If false: throw to monkey %hd\n", &new_monkey->falsemonkey);
    new_monkey->inspection_count = 0;
    return new_monkey;
}

int compare_monkeys(const void *p1, const void *p2) {
    const struct monkey **m1 = (const struct monkey **) p1;
    const struct monkey **m2 = (const struct monkey **) p2;
    return (*m2)->inspection_count - (*m1)->inspection_count;
}

int main() {
    struct monkey *monkeys[MAX_MONKEYS];
    struct monkey **current_monkey = monkeys;
    unsigned long monkeys_lcm = 1;
    while ((*current_monkey = read_new_monkey())) {
        // Burn newline
        getchar();
        register short testdivisor = (*current_monkey)->testdivisor;
        monkeys_lcm = monkeys_lcm * testdivisor / gcd(monkeys_lcm, testdivisor);
        current_monkey++;
    }
    *current_monkey = NULL;
    short num_monkeys = current_monkey - monkeys;
    for (int round = 0; round < 10000; round++) {
        for (current_monkey = monkeys; *current_monkey; current_monkey++) {
            long long inspecting_item;
            while ((inspecting_item = monkey_queue_remove(*current_monkey))) {
                (*current_monkey)->inspection_count++;
                switch ((*current_monkey)->operator) {
                    case '+':
                        inspecting_item += (*current_monkey)->operand;
                        break;
                    case '*':
                        inspecting_item *= (*current_monkey)->operand;
                        break;
                    case '$':
                        inspecting_item *= inspecting_item;
                }
                inspecting_item %= monkeys_lcm;
                if (inspecting_item % (*current_monkey)->testdivisor) {
                    monkey_queue_add(monkeys[(*current_monkey)->falsemonkey], inspecting_item);
                } else {
                    monkey_queue_add(monkeys[(*current_monkey)->truemonkey], inspecting_item);
                }
            }
        }
    }
    // Sort all monkeys in case it's required by Part 2
    qsort(monkeys, num_monkeys, sizeof(*monkeys), compare_monkeys);
    printf("%lld\n", monkeys[0]->inspection_count * monkeys[1]->inspection_count);
    for (current_monkey = monkeys; *current_monkey; current_monkey++) {
        while (monkey_queue_remove(*current_monkey));
        free(*current_monkey);
    }
    return 0;
}
