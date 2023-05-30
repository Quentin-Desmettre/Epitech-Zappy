/*
** EPITECH PROJECT, 2023
** Epitech-MyTeams
** File description:
** linked_list
*/

#include "utility/linked_list.h"
#include "utility/garbage_collector.h"
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

    if (!s)
        return;
    for (int i = 0; i < offset; i++)
        s = s->next;
    remove_if_remove_node(begin, &s, freer);
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
