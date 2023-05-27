/*
** EPITECH PROJECT, 2023
** Epitech_Zappy
** File description:
** garbage_handling
*/

#include "garbage_collector.h"

s_list_t **malloc_stack(void)
{
    static s_list_t *n = 0;

    return &n;
}

void flush_garbage(void)
{
    s_list_t *next;
    s_list_t *head = *(malloc_stack());

    while (head) {
        free(head->data);
        next = head->next;
        free(head);
        head = next;
    }
    *(malloc_stack()) = NULL;
}

void my_free(void *data)
{
    s_list_t *prev = NULL;
    s_list_t *head = *(malloc_stack());

    free(data);
    while (head) {
        if (head->data == data) {
            prev ?
            (prev->next = head->next) :
            (*(malloc_stack()) = head->next);
            free(head);
            return;
        }
        prev = head;
        head = head->next;
    }
}
