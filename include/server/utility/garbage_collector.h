/*
** EPITECH PROJECT, 2023
** Epitech_Zappy
** File description:
** garbage_collector
*/

#ifndef EPITECH_ZAPPY_GARBAGE_COLLECTOR_H
    #define EPITECH_ZAPPY_GARBAGE_COLLECTOR_H
    #include <stdlib.h>
    #include <stdbool.h>
    #define INCREMENT_IF_NOT_NULL(x) if (x) (*len)++

typedef struct s_list {
    void *data;
    struct s_list *next;
} s_list_t;

void *my_malloc(size_t size);
void *my_calloc(size_t size, size_t nmemb);
void *my_realloc(void *ptr, size_t size);
void *my_strdup(const char *str);
void remove_from_list(void *data, bool to_free);
void flush_garbage(void) __attribute__((destructor));
static inline void my_free(void *data)
{
    remove_from_list(data, true);
}

#endif //EPITECH_ZAPPY_GARBAGE_COLLECTOR_H
