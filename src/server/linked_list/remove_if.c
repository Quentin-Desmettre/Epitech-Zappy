/*
** EPITECH PROJECT, 2023
** Epitech_Zappy
** File description:
** remove_if
*/

#include "linked_list.h"
#include <stdlib.h>

bool remove_if_remove_node(list_t **list, list_t **s, void (*free_data)(void *))
{
    list_t *next;
    list_t *prev;

    if (free_data)
        free_data((*s)->data);
    if (*s == (*s)->next) {
        free(*s);
        *list = NULL;
        return false;
    }
    prev = (*s)->prev;
    next = (*s)->next;
    prev->next = next;
    next->prev = prev;
    free(*s);
    *s = next;
    return true;
}

void remove_if(list_t **list, void *data, bool (*eq_cmp)(void *, void *),
                void (*free_data)(void *))
{
    list_t *s = *list;

    if (!s)
        return;
    do {
        if (!eq_cmp(data, s->data)) {
            s = s->next;
            continue;
        }
        if (!remove_if_remove_node(list, &s, free_data))
            return;
    } while (s != *list);
}

void append_list(list_t **list, list_t *to_append, bool free_to_append)
{
    list_t *s = to_append;

    if (!s)
        return;
    do {
        append_node(list, s->data);
        s = s->next;
    } while (s != to_append);
    if (free_to_append)
        free_list(&to_append, NULL);
}
