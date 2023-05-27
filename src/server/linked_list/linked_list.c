/*
** EPITECH PROJECT, 2023
** Epitech-MyTeams
** File description:
** linked_list
*/

#include "linked_list.h"
#include "garbage_collector.h"
#include <stdlib.h>

void append_node(list_t **begin, void *data)
{
    list_t *node = my_calloc(1, sizeof(list_t));

    node->data = data;
    if (!(*begin)) {
        *begin = node;
        node->next = node;
        node->prev = node;
        return;
    }
    node->next = (*begin);
    node->prev = (*begin)->prev;
    (*begin)->prev->next = node;
    (*begin)->prev = node;
}

void remove_node(list_t **begin, int offset, void (*freer)(void *))
{
    list_t *s = *begin;
    list_t *next;
    list_t *prev;

    if (!s)
        return;
    for (int i = 0; i < offset; i++)
        s = s->next;
    if (freer)
        freer(s->data);
    if (s == s->next)
        *begin = NULL;
    else {
        prev = s->prev;
        next = s->next;
        prev->next = next;
        next->prev = prev;
        *begin = next;
    }
    my_free(s);
}

void free_list(list_t **list, void (*freer)(void *))
{
    while (*list)
        remove_node(list, 0, freer);
}

int list_size(list_t *list)
{
    int size = 0;
    list_t *s = list;

    if (!s)
        return 0;
    do {
        size++;
        s = s->next;
    } while (s != list);
    return size;
}

void push_to_front(list_t **begin, void *data)
{
    append_node(begin, data);
    *begin = (*begin)->prev;
}
