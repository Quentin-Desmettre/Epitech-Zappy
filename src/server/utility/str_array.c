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

char **str_to_word_array(const char *str, const char *delim, int *len)
{
    char **arr = NULL;
    char *tmp = NULL;
    char *saveptr = NULL;
    int i = 0;

    tmp = my_strdup(str);
    for (char *token = strtok_r(tmp, delim, &saveptr); token;
        token = strtok_r(NULL, delim, &saveptr)) {
        arr = my_realloc(arr, (i + 2) * sizeof(char *));
        arr[i] = my_strdup(token);
        i++;
    }
    arr[i] = NULL;
    my_free(tmp);
    if (len)
        *len = i;
    return arr;
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
