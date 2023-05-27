/*
** EPITECH PROJECT, 2022
** Epitech-Template
** File description:
** main.c
*/

#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include "garbage_collector.h"

int bytes_available(int fd)
{
    int bytes_available = 0;

    if (ioctl(fd, FIONREAD, &bytes_available) == -1)
        return (-1);
    return (bytes_available);
}

void append_str_array(char ***array, char *what)
{
    int len = 0;

    if (*array)
        for (; (*array)[len]; len++);
    *array = my_realloc(*array, sizeof(char *) * (len + 2));
    (*array)[len] = what;
    (*array)[len + 1] = NULL;
}

void free_str_array(char **array)
{
    if (!array)
        return;
    for (int i = 0; array[i]; i++)
        my_free(array[i]);
    my_free(array);
}

void *memdup(const void *src, size_t size)
{
    void *dst = my_calloc(1, size);

    memcpy(dst, src, size);
    return (dst);
}

char **dupstrarray(const char * const *arr)
{
    int size = 0;
    char **dup = NULL;

    for (; arr[size]; size++);
    dup = my_calloc(size + 1, sizeof(char *));
    for (int i = 0; arr[i]; i++)
        dup[i] = my_strdup(arr[i]);
    return dup;
}
