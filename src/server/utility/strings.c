/*
** EPITECH PROJECT, 2022
** Epitech-Template
** File description:
** main.c
*/

#include "utility/garbage_collector.h"
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>

char *str_concat_free(size_t *len, int nb_strings, ...)
{
    va_list ap;
    char *str = NULL;
    char *tmp;

    *len = 0;
    va_start(ap, nb_strings);
    for (int i = 0; i < nb_strings; i++) {
        tmp = va_arg(ap, char *);
        len += strlen(tmp);
    }
    va_end(ap);
    str = my_calloc(*len + 1, sizeof(char));
    va_start(ap, nb_strings);
    for (int i = 0; i < nb_strings; i++) {
        tmp = va_arg(ap, char *);
        strcat(str, tmp);
        my_free(tmp);
    }
    va_end(ap);
    return str;
}

char *str_concat(size_t *len, int nb_strings, ...)
{
    va_list ap;
    char *str = NULL;
    char *tmp;

    *len = 0;
    va_start(ap, nb_strings);
    for (int i = 0; i < nb_strings; i++) {
        tmp = va_arg(ap, char *);
        len += strlen(tmp);
    }
    va_end(ap);
    str = my_calloc(*len + 1, sizeof(char));
    va_start(ap, nb_strings);
    for (int i = 0; i < nb_strings; i++) {
        tmp = va_arg(ap, char *);
        strcat(str, tmp);
    }
    va_end(ap);
    return str;
}

bool str_ends_with(const char *str, const char *suffix)
{
    size_t str_len = strlen(str);
    size_t suffix_len = strlen(suffix);

    if (suffix_len > str_len)
        return false;
    return strcmp(str + str_len - suffix_len, suffix) == 0;
}

void str_append_free(char **str, size_t *len, char *append)
{
    size_t append_len = strlen(append);

    *str = my_realloc(*str, *len + 1);
    memcpy(*str + *len, append, append_len + 1);
    *len += append_len;
    my_free(append);
}

void *memdup(const void *src, size_t size)
{
    void *dst = my_calloc(1, size);

    memcpy(dst, src, size);
    return (dst);
}
