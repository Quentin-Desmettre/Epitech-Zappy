/*
** EPITECH PROJECT, 2023
** Epitech_Zappy
** File description:
** str_array
*/

#include "utility/garbage_collector.h"
#include <string.h>
#include <errno.h>

char *perror_str(const char *str)
{
    char *err = my_calloc(1, strlen(str) + strlen(strerror(errno)) + 3);

    strcat(err, str);
    strcat(err, ": ");
    strcat(err, strerror(errno));
    return err;
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

char **dupstrarray(const char **arr)
{
    int size = 0;
    char **dup = NULL;

    for (; arr[size]; size++);
    dup = my_calloc(size + 1, sizeof(char *));
    for (int i = 0; arr[i]; i++)
        dup[i] = my_strdup(arr[i]);
    return dup;
}

char **split_on(char *str, char *delim, int *len)
{
    char **array = NULL;
    char *occ;
    char *tmp;

    if (len)
        *len = 0;
    INCREMENT_IF_NOT_NULL(len);
    if (!strpbrk(str, delim))
        return dupstrarray((const char *[]){str, NULL});
    while ((occ = strpbrk(str, delim))) {
        tmp = strndup(str, occ - str);
        append_str_array(&array, my_strdup(tmp));
        free(tmp);
        str = occ + 1;
        INCREMENT_IF_NOT_NULL(len);
    }
    append_str_array(&array, my_strdup(str));
    return array;
}
