#pragma once

#ifdef __KERNEL__
    #include <linux/slab.h>
    #include <linux/uaccess.h>
    #include <linux/limits.h>
    #include <linux/errno.h>
    #include <linux/time.h>
    #include <linux/types.h>

    #define ALLOCATE(size) kmalloc(size, GFP_KERNEL)
    #define DEALLOCATE(ptr) kfree(ptr)
    #define COPY_TO_USER(to, from, size) copy_to_user(to, from, size)
    #define COPY_FROM_USER(to, from, size) copy_from_user(to, from, size)
    #define TIME_SEED ktime_get_ns()
#else
    #include <string.h>
    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>
    #include <limits.h>
    #include <errno.h>
    #include <time.h>
    #define ALLOCATE(size) malloc(size)
    #define DEALLOCATE(ptr) free(ptr)
    #define COPY_TO_USER(to, from, size) memcpy(to, from, size)
    #define COPY_FROM_USER(to, from, size) memcpy(to, from, size)
    #define TIME_SEED time(NULL)
#endif
