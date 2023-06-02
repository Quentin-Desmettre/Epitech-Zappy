/*
** EPITECH PROJECT, 2023
** Epitech_Zappy
** File description:
** remove_if
*/

#include "utility/linked_list.h"
#include <stdlib.h>
#include "utility/garbage_collector.h"

bool remove_if_remove_node(list_t **begin,
        list_t **s, void (*freer)(void *))
{
    list_t *next;
    list_t *prev;

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

void remove_if(list_t **list, void *data, bool (*eq_cmp)(void *, void *),
                void (*free_data)(void *))
{
    list_t *s = *list;

    if (!s)
        return;
label:
    do {
        if (!((!eq_cmp && data == s->data) ||
        (eq_cmp && eq_cmp(data, s->data)))) {
            s = s->next;
            continue;
        }
        if (!remove_if_remove_node(list, &s, free_data))
            return;
        go_to label;
    } while (s != *list);
}

void append_list(list_t **list, list_t *to_append,
                    bool free_to_append, void (*freer)(void *))
{
    list_t *s = to_append;

    if (!s)
        return;
    do {
        append_node(list, s->data);
        s = s->next;
    } while (s != to_append);
    if (free_to_append)
        free_list(&to_append, freer);
}
