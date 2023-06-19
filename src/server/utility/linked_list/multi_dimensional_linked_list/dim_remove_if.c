/*
** EPITECH PROJECT, 2023
** Epitech_Zappy
** File description:
** remove_if
*/

#include "utility/linked_list.h"
#include <stdlib.h>
#include "utility/garbage_collector.h"

bool dim_remove_if_remove_node(dim_list_t **begin,
    dim_list_t **s, void (*freer)(void *))
{
    dim_list_t *next;
    dim_list_t *prev;

    if (freer)
        freer((*s)->data);
    if ((*s) == (*s)->next) {
        *begin = NULL;
        my_free((*s));
        return false;
    }
    prev = (*s)->prev;
    next = (*s)->next;
    prev->next = next;
    next->prev = prev;
    *begin = next;
    my_free((*s));
    *s = next;
    return true;
}

void dim_remove_if(dim_list_t **list, void *data,
        bool (*eq_cmp)(void *, void *), void (*free_data)(void *))
{
    dim_list_t *s = *list;
    bool cont = true;

    if (!s)
        return;
    while (cont) {
        if (((!eq_cmp && data == s->data) ||
            (eq_cmp && eq_cmp(data, s->data)))) {
            cont = dim_remove_if_remove_node(list, &s, free_data);
            continue;
        }
        s = s->next;
        if (s == *list)
            return;
    }
}

void dim_append_list(dim_list_t **list, dim_list_t *to_append,
    bool free_to_append, void (*freer)(void *))
{
    dim_list_t *s = to_append;

    if (!s)
        return;
    do {
        dim_append_node(list, s->data);
        s = s->next;
    } while (s != to_append);
    if (free_to_append)
        dim_free_list(&to_append, freer);
}
