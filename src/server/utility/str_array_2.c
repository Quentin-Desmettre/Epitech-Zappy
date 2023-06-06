/*
** EPITECH PROJECT, 2023
** Epitech-Zappy
** File description:
** str_array_2
*/

#include "utility/strings.h"
#include <string.h>

static bool has_duplicates_inner_loop(char **arr, char *str, int i)
{
    for (int j = i + 1; arr[j]; j++)
        if (strcmp(str, arr[j]) == 0)
            return true;
    return false;
}

bool has_duplicates(char **arr)
{
    for (int i = 0; arr[i]; i++)
        if (has_duplicates_inner_loop(arr, arr[i], i))
            return true;
    return false;
}
