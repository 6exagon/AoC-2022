#include <stdio.h>

#define MINUTES 32
#define STRING "Blueprint %hu: Each ore robot costs %hu ore. Each clay robot costs %hu ore. Each obsidian robot costs %hu ore and %hu clay. Each geode robot costs %hu ore and %hu obsidian.\n"
#define STEP s.ore += s.ore_robots; s.clay += s.clay_robots; s.obsidian += s.obsidian_robots; s.geodes += s.geode_robots; if (!--turns) return s.geodes;

struct blueprint {
    unsigned short id;
    unsigned short ore_orecost;
    unsigned short clay_orecost;
    unsigned short obsidian_orecost;
    unsigned short obsidian_claycost;
    unsigned short geode_orecost;
    unsigned short geode_obsidiancost;
};

struct state {
    unsigned short ore;
    unsigned short clay;
    unsigned short obsidian;
    unsigned short geodes;
    unsigned short ore_robots;
    unsigned short clay_robots;
    unsigned short obsidian_robots;
    unsigned short geode_robots;
};

enum resource { ORE, CLAY, OBSIDIAN, GEODE };

unsigned int choose(struct blueprint b, struct state s, enum resource r, unsigned short turns) {
    switch (r) {
        case ORE:
            while (s.ore < b.ore_orecost) {
                STEP
            }
            s.ore -= b.ore_orecost;
            STEP
            s.ore_robots++;
            break;
        case CLAY:
            while (s.ore < b.clay_orecost) {
                STEP
            }
            s.ore -= b.clay_orecost;
            STEP
            s.clay_robots++;
            break;
        case OBSIDIAN:
            while (s.ore < b.obsidian_orecost || s.clay < b.obsidian_claycost) {
                STEP
            }
            s.ore -= b.obsidian_orecost;
            s.clay -= b.obsidian_claycost;
            STEP
            s.obsidian_robots++;
            break;
        case GEODE:
            while (s.ore < b.geode_orecost || s.obsidian < b.geode_obsidiancost) {
                STEP
            }
            s.ore -= b.geode_orecost;
            s.obsidian -= b.geode_obsidiancost;
            STEP
            s.geode_robots++;
    }
    if (turns < 2) {
        STEP
    }
    unsigned int max = choose(b, s, ORE, turns);
    unsigned int new = choose(b, s, CLAY, turns);
    max = (new > max) ? new : max;
    if (s.obsidian_robots) {
        new = choose(b, s, GEODE, turns);
        max = (new > max) ? new : max;
    }
    if (s.clay_robots) {
        new = choose(b, s, OBSIDIAN, turns);
        max = (new > max) ? new : max;
    }
    return max;
}

int main() {
    unsigned int total_score = 1;
    struct blueprint b;
    struct state s = {0};
    s.ore_robots++;
    for (short x = 0; x < 3; x++) {
        scanf(
              STRING,
              &b.id,
              &b.ore_orecost,
              &b.clay_orecost,
              &b.obsidian_orecost,
              &b.obsidian_claycost,
              &b.geode_orecost,
              &b.geode_obsidiancost);
        unsigned int ore_first = choose(b, s, ORE, MINUTES);
        unsigned int clay_first = choose(b, s, CLAY, MINUTES);
        total_score *= (ore_first > clay_first) ? ore_first : clay_first;
    }
    printf("%u\n", total_score);
    return 0;
}
