/*
** EPITECH PROJECT, 2023
** Epitech-Zappy
** File description:
** data_send
*/

#include "server.h"

struct timespec timespec_add(struct timespec a, struct timeval b)
{
    struct timespec res;

    res.tv_sec = a.tv_sec + b.tv_sec;
    res.tv_nsec = a.tv_nsec + b.tv_usec * 1000;
    if (res.tv_nsec >= 1000000000L) {
        res.tv_sec++;
        res.tv_nsec -= 1000000000L;
    }
    return res;
}

int action_cmp(const void *a, const void *b)
{
    action_t *action_a = *(action_t **)a;
    action_t *action_b = *(action_t **)b;
    long sec_diff = action_a->end_time.tv_sec - action_b->end_time.tv_sec;
    long ns_diff = action_a->end_time.tv_nsec - action_b->end_time.tv_nsec;

    return (int)(sec_diff ? sec_diff : ns_diff);
}
