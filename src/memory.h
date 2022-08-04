#pragma once
#include <stddef.h>

void *xmalloc(size_t size);
void *xcalloc(size_t num, size_t size);
void *xrealloc(void *ptr, size_t new_size);
void xfree(void *ptr);
