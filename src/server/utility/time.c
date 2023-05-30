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
