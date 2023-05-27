/*
** EPITECH PROJECT, 2023
** Epitech_Zappy
** File description:
** garbage_collector
*/

#ifndef EPITECH_ZAPPY_GARBAGE_COLLECTOR_H
    #define EPITECH_ZAPPY_GARBAGE_COLLECTOR_H
    #include <stdlib.h>

typedef struct s_list {
    void *data;
    struct s_list *next;
} s_list_t;

void *my_malloc(size_t size);
void *my_calloc(size_t size, size_t nmemb);
void *my_realloc(void *ptr, size_t size);
void my_free(void *ptr);
void *my_strdup(const char *str);
void flush_garbage(void) __attribute__((destructor));

#endif //EPITECH_ZAPPY_GARBAGE_COLLECTOR_H
