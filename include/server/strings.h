/*
** EPITECH PROJECT, 2023
** Epitech_Zappy
** File description:
** strings
*/

#ifndef EPITECH_ZAPPY_STRINGS_H
    #define EPITECH_ZAPPY_STRINGS_H
    #include <stddef.h>

void append_str_array(char ***array, char *what);
void free_str_array(char **array);
void *memdup(const void *src, size_t size);
int bytes_available(int fd);
char **dupstrarray(const char * const *arr);

#endif //EPITECH_ZAPPY_STRINGS_H
