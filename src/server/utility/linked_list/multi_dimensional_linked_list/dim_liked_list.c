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
