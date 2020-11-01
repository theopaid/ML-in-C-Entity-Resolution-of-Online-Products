#include "../hdr/includes.h"

void *safe_malloc(size_t size)
{
    void *ptr = malloc(size);

    if (!ptr && (size > 0))
    {
        perror("malloc failed!");
        exit(EXIT_FAILURE);
    }

    return ptr;
}

void *safe_calloc(long elementsNum, size_t size)
{
    void *ptr = calloc(elementsNum, size);

    if (!ptr && (size > 0) && (elementsNum > 0))
    {
        perror("calloc failed!");
        exit(EXIT_FAILURE);
    }

    return ptr;
}