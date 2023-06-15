/*
** EPITECH PROJECT, 2023
** Epitech-Zappy
** File description:
** strings2
*/

#include "server.h"
#include <stdarg.h>

int str_array_len(char **array)
{
    int i = 0;

    if (!array)
        return 0;
    while (array[i] != 0)
        i++;
    return i;
}

char *my_asprintf(const char *format, ...)
{
    va_list ap;
    char *str = NULL;
    char *dup;

    va_start(ap, format);
    if (vasprintf(&str, format, ap) == -1) {
        perror("vasprintf");
        exit(84);
    }
    va_end(ap);
    dup = my_strdup(str);
    free(str);
    return dup;
}

void *memdup(const void *src, size_t size)
{
    void *dst = my_calloc(1, size);

    memcpy(dst, src, size);
    return (dst);
}
