/*
** EPITECH PROJECT, 2023
** Epitech-Zappy
** File description:
** str_array_2
*/

#include "utility/strings.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>

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

int bytes_available(int fd)
{
    int bytes_available = 0;

    if (ioctl(fd, FIONREAD, &bytes_available) == -1)
        return -1;
    return (bytes_available);
}

struct timeval timespec_diff(struct timespec a, struct timespec b)
{
    struct timeval res = {a.tv_sec - b.tv_sec, (a.tv_nsec - b.tv_nsec) / 1000};

    if (res.tv_usec < 0) {
        res.tv_sec--;
        res.tv_usec += 1000000;
    }
    if (res.tv_usec > 1000000) {
        res.tv_sec++;
        res.tv_usec -= 1000000;
    }
    if (res.tv_sec < 0) {
        res.tv_sec = 0;
        res.tv_usec = 0;
    }
    return res;
}

bool str_array_contains(char **arr, char *str)
{
    for (int i = 0; arr[i]; i++)
        if (!strcmp(arr[i], str))
            return true;
    return false;
}
