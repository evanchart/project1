//
// Created by Evan Hart on 10/12/19.
//

#ifndef PROJECT1_SKIPLIST_H
#define PROJECT1_SKIPLIST_H

typedef struct node {
    int value;
    int key;

    struct node **front;
} node;

typedef struct skiplist {
    int size;
    int level;

    struct node *head;
} skiplist;

static unsigned int generate_random_int(void);
static void seed_random(unsigned int seed);
int random_level(void);
#endif //PROJECT1_SKIPLIST_H
