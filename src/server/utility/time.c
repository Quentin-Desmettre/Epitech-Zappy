/*
** EPITECH PROJECT, 2023
** Epitech_Zappy
** File description:
** time
*/

#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utility/strings.h"
#include <stdbool.h>

struct timespec get_end_time(double ticks, int freq, struct timespec now)
{
    double duration = ticks / freq;
    struct timespec tmp = {
            now.tv_sec + (time_t)duration,
            now.tv_nsec + (time_t)((duration - (time_t)(duration)) * S_TO_NS)
    };

    while (tmp.tv_nsec >= 1000000000L) {
        tmp.tv_sec++;
        tmp.tv_nsec -= 1000000000L;
    }
    return tmp;
}

void get_time(struct timespec *timeout)
{
    if (clock_gettime(CLOCK_MONOTONIC, timeout) == -1) {
        perror("clock_gettime");
        exit(84);
    }
}

int try_select(int fd_max, fd_set *read_fds,
                fd_set *write_fds, struct timeval *timeout)
{
    return select(fd_max, read_fds, write_fds, NULL, timeout);
}

bool is_timespec_less(struct timespec *t1, struct timespec *t2)
{
    if (t1->tv_sec < t2->tv_sec)
        return (true);
    if (t1->tv_sec == t2->tv_sec && t1->tv_nsec < t2->tv_nsec)
        return (true);
    return (false);
}

bool is_timespec_equal(struct timespec *t1, struct timespec *t2)
{
    return (t1->tv_sec == t2->tv_sec && t1->tv_nsec == t2->tv_nsec);
}
