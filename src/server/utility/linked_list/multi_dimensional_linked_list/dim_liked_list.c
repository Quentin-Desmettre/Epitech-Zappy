/*
** EPITECH PROJECT, 2023
** Epitech-MyTeams
** File description:
** linked_list
*/

#include "utility/linked_list.h"
#include "utility/garbage_collector.h"
#include <stdlib.h>

void dim_append_node(dim_list_t **begin, void *data)
{
    dim_list_t *node = my_calloc(1, sizeof(dim_list_t));

    node->data = data;
    if (!(*begin)) {
        *begin = node;
        node->next = node;
        node->prev = node;
        node->over = NULL;
        node->under = NULL;
        return;
    }
    node->next = (*begin);
    node->prev = (*begin)->prev;
    (*begin)->prev->next = node;
    (*begin)->prev = node;
    node->over = NULL;
    node->under = NULL;
}

void dim_remove_node(dim_list_t **begin, int offset, void (*freer)(void *))
{
    dim_list_t *s = *begin;

    if (!s)
        return;
    for (int i = 0; i < offset; i++)
        s = s->next;
    dim_remove_if_remove_node(begin, &s, freer);
}

void dim_free_list(dim_list_t **list, void (*freer)(void *))
{
    while (*list)
        dim_remove_node(list, 0, freer);
}

int dim_list_size(dim_list_t *list, enum dimension dim)
{
    int size = 0;
    dim_list_t *s = list;

    if (!s)
        return 0;
    do {
        size++;
        if (dim)
            s = s->next;
        else
            s = s->over;
    } while (s != list);
    return size;
}

void dim_push_to_front(dim_list_t **begin, void *data)
{
    dim_append_node(begin, data);
    *begin = (*begin)->prev;
}
