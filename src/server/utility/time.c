/*
** EPITECH PROJECT, 2023
** Epitech_Zappy
** File description:
** time
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/ioctl.h>
#include <stdbool.h>

int bytes_available(int fd)
{
    int bytes_available = 0;

    if (ioctl(fd, FIONREAD, &bytes_available) == -1) {
        perror("ioctl");
        exit(84);
    }
    return (bytes_available);
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
    int ret = select(fd_max, read_fds, write_fds, NULL, timeout);

    if (ret == -1) {
        perror("select");
        exit(84);
    }
    return ret;
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
