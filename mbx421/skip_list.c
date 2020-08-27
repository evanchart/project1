#include "skip_list.h"
#include <linux/list.h>
#include "cred.h"


static unsigned int next_random = 9001;

static void seed_random(unsigned int seed) {
    next_random = seed;
}

static unsigned int generate_random_int(void) {
    next_random = next_random * 1103515245 + 12345;
    return (next_random / 65536) % 32768;
}

int random_level(skiplist *list) {
    int level = 1;

    while (generate_random_int() < 32767 / list->prob && level < list->max) {
        level++;
    }
    return level;
}

int message_length(skiplist *list, unsigned long id) {
    node_t *x;
    message_t *y;
    x = skiplist_search(list, id);
    y = list_last_entry(&x->messages, message_t, list_node);
    return sizeof(y->msg);
}

int message_send(skiplist *list, unsigned long id, const unsigned char *msg) {
    if (skiplist_exists(list, id) == 0) {
        node_t *x;
        x = skiplist_search(list, id);

        message_t *y;
        y = (message_t*)malloc(sizeof(message_t));
        y->msg = msg;
        list_add(&y->list_node, &x->messages);

        x->mbx_size++;
        return 0;
    } else {
        return 1;
    }
}

unsigned char *message_receive(skiplist *list, unsigned long id) {
    message_t *y;
    message_t *y_temp;
    const unsigned char *msg;
    if (skiplist_exists(list, id) == 0) {
        node_t *x;
        x = skiplist_search(list, id);

        y = list_last_entry(&x->messages, message_t, list_node);
        y_temp = y;
        list_del(&y->list_node);
        free(y);
        x->mbx_size--;
        return y_temp;
    } else {
        return 0;
    }
}

int mailbox_destroy(skiplist *list, unsigned long id) {
    if (skiplist_exists(list, id) == 0) {
        message_t *y;
        message_t *y_temp;

        node_t *x;
        x = skiplist_search(list, id);
        list_for_each_entry_safe(y, y_temp, &x->messages, list_node) {
            list_del(&y->list_node);
            free(y);
        }
        skiplist_delete(list, x->id);
        return 0;
    } else {
        return 1;
    }
}

int skiplist_shutdown(skiplist *list) {
    if(list->level != 0) {
        node_t *x = list->head;
        node_t *temp;
        while (x->front[1] != list->head && x) {
            temp = x;
            x = x->front[1];
            mailbox_destroy(list, temp->id);
        }
        list->head = NULL;
        list->max = 0;
        list->prob = 0;
        list->size = 0;
        list->level = 0;
        return 0;
    }
    else {
        return 1;
    }

}


skiplist *skiplist_create(skiplist *list, unsigned int max, unsigned int prob) {
    node_t *head = (mnode *) malloc(sizeof(struct mnode));
    list->head = head;
    list->max = max;
    list->prob = prob;
    head->id = 2147483647;
    head->front = (mnode **) malloc(sizeof(mnode*)*(list->max+1));

    seed_random(next_random);

    for(int i = 0; i <= list->max; i++) {
        head->front[i] = list->head;
    }

    list->level = 1;
    list->size = 0;

    return list;
}

int skiplist_insert(skiplist *list, unsigned long id) {

    node_t *update[list->max + 1];
    node_t *x;
    x = list->head;
    int level, i;

    for (i = list->level; i >= 1; i--) {
        while(x->front[i]->id < id) {
            x = x->front[i];
        }
        update[i] = x;
    }

    x = x->front[1];

    if (id == x->id) {
        return 0;
    } else {
        level = random_level(list);

        if (level > list->level) {
            for (i = list->level + 1; i <= level; i++) {
                update[i] = list->head;
            }
            list->level = level;
        }

        x = (node_t *) malloc(sizeof(node_t));
        x->front = (node_t **) malloc(sizeof(node_t*)*level+1);

        x->id = id;
        x->mbx_size = 0;
        INIT_LIST_HEAD(&x->messages);

        for (i = 1; i <= level; i++) {
            x->front[i] = update[i]->front[i];
            update[i]->front[i] = x;
        }

    }
    return 0;
}

int skiplist_delete(skiplist *list, unsigned long id) {
    node_t *update[list->max+ 1];
    node_t *x;
    x = list->head;

    int i;
    for (i = list->level; i >= 1; i--) {
        while(x->front[i]->id < id) {
            x = x->front[i];
        }
        update[i] = x;
    }

    x = x->front[1];
    if (x->id == id) {
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

node_t *skiplist_search(skiplist *list, unsigned long id) {
    node_t *x;
    x = list->head;

    int i;
    for (i = list->level; i>= 1; i--) {
        while (x->front[i]->id < id) {
            x = x->front[i];
        }
    }
    if (x->front[1]->id == id) {
        return x->front[1];
    } else {
        return NULL;
    }
}

int skiplist_exists(skiplist *list, unsigned long id) {
    node_t *x;
    x = list->head;

    int i;
    for (i = list->level; i>= 1; i--) {
        while (x->front[i]->id < id) {
            x = x->front[i];
        }
    }
    if (x->front[1]->id == id) {
        return 0;
    } else {
        return 1;
    }
}

int mailbox_size(skiplist *list, unsigned long id) {
    node_t *x;
    x = skiplist_search(list, id);
    return x->mbx_size;
}

int skiplist_size(skiplist *list) {
    return list->size;
}
