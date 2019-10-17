#include <stdio.h>
#include <stdlib.h>
#include "skiplist.h"
#include <time.h>

#define SKIP_MAX_LVL 8

static unsigned int next_random = 9001;

static unsigned int generate_random_int(void) {
    next_random = next_random * 1103515245 + 12345;
    return (int) (next_random / 65536) % 32768;
}

void seed_random(unsigned int seed) {
    next_random = seed;
}


int random_level(void) {
    int level;
    level = 0;
    time_t t;
    while (level <= 327267) {
        level = (int) generate_random_int();
        if (level > 327267) {
            break;
        }
        else {
            level++;
            seed_random((unsigned) time(&t));
        }
    }
    return level;
}

skiplist *skiplist_create(skiplist *list) {
    node *head = (node *) malloc(sizeof(struct node));
    list->head = head;
    head->key = 1001;
    head->front = (node **) malloc(sizeof(node*)*(SKIP_MAX_LVL+1));

    for(int i = 0; i <= SKIP_MAX_LVL; i++) {
        head->front[i] = list->head;
    }

    list->level = 1;
    list->size = 0;

    return list;
}

int skiplist_insert(skiplist *list, int key, int value) {

    node *update[SKIP_MAX_LVL + 1];
    node *x;
    x = list->head;
    int level, i;

    for (i = list->level; i >= 1; i--) {
        while(x->front[i]->key < key) {
            x = x->front[i];
        }
        update[i] = x;
    }

    x = x->front[1];

    if (key == x->key) {
        x->value = value;
        return 0;
    } else {
        level = random_level();

        if (level > list->level) {
            for (i = list->level + 1; i <= level; i++) {
                update[i] = list->head;
            }
            list->level = level;
        }

        x = (node *) malloc(sizeof(node));
        x->front = (node **) malloc(sizeof(node*)*level+1);

        x->key = key;
        x->value = value;

        for (i = 1; i <= level; i++) {
            x->front[i] = update[i]->front[i];
            update[i]->front[i] = x;
        }

    }
    return 0;
}

int skiplist_delete(skiplist *list, int key) {
    node *update[SKIP_MAX_LVL + 1];
    node *x;
    x = list->head;

    int i;
    for (i = list->level; i >= 1; i--) {
        while(x->front[i]->key < key) {
            x = x->front[i];
        }
        update[i] = x;
    }

    x = x->front[1];
    if (x->key == key) {
        for (i = 1; i <= list->level; i++) {
            if (update[i]->front[i] != x) {
                break;
            }
            update[i]->front[1] = x->front[i];
        }

        free(x->front);
        free(x);
        while(list->level > 1 && list->head->front[list->level] == list->head) {
            list->level = list->level - 1;
        }
        return 0;
    }
    return 1;
}

node *skiplist_search(skiplist *list, int key) {
    node *x;
    x = list->head;

    int i;
    for (i = list->level; i>= 1; i--) {
        while (x->front[i]->key < key) {
            x = x->front[i];
        }
    }
    if (x->front[1]->key == key) {
        return x->front[1];
    } else {
        return NULL;
    }
}

