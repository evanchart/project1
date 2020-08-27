#ifndef PROJECT1_SKIPLIST_H
#define PROJECT1_SKIPLIST_H


typedef struct message {

    const unsigned char *msg;

    struct list_head list_node;
} message_t;

typedef struct mnode {
    unsigned long id;
    int mbx_size;

    struct mnode **front;


    struct list_head messages;
} node_t;

typedef struct skiplist {

    int size;
    int level;

    unsigned int max;
    unsigned int prob;

    struct mnode *head;
} skiplist;


static void seed_random(unsigned int seed);
static unsigned int generate_random_int(void);
int random_level(skiplist *list);

int message_length(skiplist *list, unsigned long id);
int message_send(skiplist *list, unsigned long id, const unsigned char *msg);
int mailbox_size(skiplist *list, unsigned long id);
unsigned char *message_receive(skiplist *list, unsigned long id);
int mailbox_destroy(skiplist *list, unsigned long id);
int skiplist_shutdown(skiplist *list);
skiplist *skiplist_create(skiplist *list, unsigned int max, unsigned int prob);
int skiplist_insert(skiplist *list, unsigned long id);
int skiplist_delete(skiplist *list, unsigned long id);
node_t *skiplist_search(skiplist *list, unsigned long id);
int skiplist_exists(skiplist *list, unsigned long id);
int skiplist_size(skiplist *list);



#endif //PROJECT1_SKIPLIST_H
