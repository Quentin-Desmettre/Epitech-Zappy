/*
** EPITECH PROJECT, 2021
** lib
** File description:
** my_malloc.c
*/

#include <stdlib.h>
#include <string.h>
#include "utility/garbage_collector.h"
#include <stdio.h>

s_list_t **malloc_stack(void);

static void push_to_stack(s_list_t **stack, void *data)
{
    s_list_t *node = malloc(sizeof(s_list_t));

    if (!node) {
        perror("malloc");
        exit(84);
    }
    node->data = data;
    node->next = *stack;
    *stack = node;
}

void *my_malloc(size_t size)
{
    void *r = malloc(size);

    if (!r) {
        perror("malloc");
        exit(84);
    }
    push_to_stack(malloc_stack(), r);
    return r;
}

void *my_calloc(size_t nmemb, size_t size)
{
    void *r = calloc(nmemb, size);

    if (!r) {
        perror("calloc");
        exit(84);
    }
    push_to_stack(malloc_stack(), r);
    return r;
}

void *my_realloc(void *ptr, size_t size)
{
    void *r = my_malloc(size);

    memcpy(r, ptr, size);
    my_free(ptr);
    return r;
}

void *my_strdup(char const *src)
{
    char *r = my_malloc(strlen(src) + 1);

    return strcpy(r, src);
}
