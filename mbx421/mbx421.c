#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/errno.h>
#include <linux/cred.h>

#include "u2k.h"
#include "skip_list.h"

struct skiplist *SKIP_LIST = NULL;

SYSCALL_DEFINE2(mbx421_init, unsigned int, ptrs, unsigned int, prob) {
/*
    if(getuid!=0) {
        return -EPERM;
    }
    else */
if (prob != 2 || prob != 4 || prob != 8 || prob != 16) {
        return -EINVAL;
    }
    else if (ptrs == 0){
        return -EINVAL;
    }
    else if (SKIP_LIST->level != 0) {
        return -EEXIST;
    }
    else {
        SKIP_LIST = skiplist_create(SKIP_LIST, ptrs, prob);
    }
}

SYSCALL_DEFINE0(mbx421_shutdown) {
/*
    if(getuid!=0) {
        return -EPERM;
    }
    else*/ if (SKIP_LIST->level == 0) {
        return -ENOENT;
    }
    else {
        return skiplist_shutdown(SKIP_LIST);
    }
}

SYSCALL_DEFINE1(mbx421_create, unsigned long, id) {
    /*if(getuid!=0) {
        return -EPERM;
    }
    else*/ if (SKIP_LIST->level == 0) {
        return -ENOENT;
    }
    else if (skiplist_exists(SKIP_LIST, id) == 0) {
        return -EEXIST;
    }
    else if (id == 0 || id > ((2^64)-1)) {
        return -EINVAL;
    }
    else {
        return skiplist_insert(SKIP_LIST, id);
    }
}

SYSCALL_DEFINE1(mbx421_destroy, unsigned long, id) {
    /*if(getuid!=0) {
        return -EPERM;
    }
    else*/ if (id == 0 || id > (ULONG_MAX)) {
        return -EINVAL;
    }
    else if (SKIP_LIST->level == 0) {
        return -ENODEV;
    }
    else if (skiplist_exists(SKIP_LIST, id) == 1)  {
        return -ENOENT;
    }
    else {
        return mailbox_destroy(SKIP_LIST, id);
    }
}

SYSCALL_DEFINE1(mbx421_count, unsigned long, id) {
    if (id == 0 || id > (ULONG_MAX)) {
        return -EINVAL;
    }
    else if(SKIP_LIST->level == 0) {
        return -ENODEV;
    }
    else if (skiplist_exists(SKIP_LIST, id) == 1) {
        return -ENOENT;
    }
    else {
        return mailbox_size(SKIP_LIST, id);
    }
}

SYSCALL_DEFINE3(mbx421_send, unsigned long, id, const unsigned char __user *, msg, long, len) {
    if (id == 0 || id > (ULONG_MAX)) {
        return -EINVAL;
    }
	write_lock(&lock);
    else if(SKIP_LIST->level == 0) {
        return -ENODEV;
    }
    else if (skiplist_exists(SKIP_LIST, id) == 1) {
        return -ENOENT;
    }
    else {
	message_t *y = (message_t *)ALLOCATE(sizeof(struct message));
	y->msg = ALLOCATE(len *sizeof(unsigned char));
	unsigned long i = COPY_FROM_USER(y->msg, msg, len);
	int j = message_send(SKIP_LIST, id, y->msg);
	write_unlock(&lock);
	return i;
    }
}

SYSCALL_DEFINE3(mbx421_recv, unsigned long, id, unsigned char __user *, msg, long, len) {
	return 0;
}

SYSCALL_DEFINE1(mbx421_length, unsigned long, id) {
    if (id == 0 || id > (ULONG_MAX)) {
        return -EINVAL;
    }
    else if(SKIP_LIST->level == 0) {
        return -ENODEV;
    }
    else if (skiplist_exists(SKIP_LIST, id) == 1) {
        return -ENOENT;
    }
    else {
        return message_length(SKIP_LIST, id);
    }

}

SYSCALL_DEFINE2(mbx421_acl_add, unsigned long, id, pid_t, process_id) {
	return 0;
}

SYSCALL_DEFINE2(mbx421_acl_add, unsigned long, id, pid_t, process_id) {
	return 0;
}
