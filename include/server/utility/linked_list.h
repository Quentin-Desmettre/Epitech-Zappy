/*
** EPITECH PROJECT, 2023
** Epitech-MyTeams
** File description:
** linked_list
*/

#ifndef LINKED_LIST_H_
    #define LINKED_LIST_H_
    #include <stdbool.h>
    #include <sys/select.h>

typedef struct list {
    void *data;
    struct list *next;
    struct list *prev;
} list_t;

typedef struct dim_list {
    void *data;
    struct dim_list *next;
    struct dim_list *prev;
    struct dim_list *over;
    struct dim_list *under;
} dim_list_t;

enum dimension {
    VERTICAL,
    HORIZONTAL
};

void append_node(list_t **list, void *data);
void remove_node(list_t **list, int index, void (*free_data)(void *));
void free_list(list_t **list, void (*free_data)(void *));
int list_size(list_t *list);
void push_to_front(list_t **begin, void *data);
void remove_if(list_t **list, void *data, bool (*cmp)(void *, void *),
    void (*free_data)(void *));
void append_list(list_t **list, list_t *to_append, bool free_to_append,
    void (*free_data)(void *));
bool remove_if_remove_node(list_t **list,
        list_t **s, void (*free_data)(void *));

void dim_append_node(dim_list_t **list, void *data);
void dim_remove_node(dim_list_t **list, int index, void (*free_data)(void *));
void dim_free_list(dim_list_t **list, void (*free_data)(void *));
int dim_list_size(dim_list_t *list, enum dimension dim);
void dim_push_to_front(dim_list_t **begin, void *data);
void dim_remove_if(dim_list_t **list, void *data, bool (*cmp)(void *, void *),
    void (*free_data)(void *));
void dim_append_list(dim_list_t **list, dim_list_t *to_append, bool free_to_append,
    void (*free_data)(void *));
bool dim_remove_if_remove_node(dim_list_t **list,
    dim_list_t **s, void (*free_data)(void *));

#endif /* !LINKED_LIST_H_ */
