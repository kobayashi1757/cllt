#include <stdio.h>
#include <stdlib.h>
#include "memory.h"

void *xmalloc(size_t size) {
    void *ptr = malloc(size);
    if (ptr == NULL) {
        fprintf(stderr, "Out of memory");
        exit(1);
    }
    return ptr;
}

void *xcalloc(size_t num, size_t size) {
    void *ptr = calloc(num, size);
    if (ptr == NULL) {
        fprintf(stderr, "Out of memory");
        exit(1);
    }
    return ptr;
}

void *xrealloc(void *ptr, size_t new_size) {
    ptr = realloc(ptr, new_size);
    if (ptr == NULL) {
        fprintf(stderr, "Out of memory");
        exit(1);
    }
    return ptr;
}

void xfree(void *ptr) {
    free(ptr);
}
