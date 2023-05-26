/*
** EPITECH PROJECT, 2023
** Epitech-MyTeams
** File description:
** linked_list
*/

#ifndef LINKED_LIST_H_
    #define LINKED_LIST_H_

typedef struct list {
    void *data;
    struct list *next;
    struct list *prev;
} list_t;

void append_node(list_t **list, void *data);
void remove_node(list_t **list, int index, void (*free_data)(void *));
void free_list(list_t **list, void (*free_data)(void *));
int list_size(list_t *list);
void push_to_front(list_t **begin, void *data);

#endif /* !LINKED_LIST_H_ */
