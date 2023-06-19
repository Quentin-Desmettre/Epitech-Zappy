/*
** EPITECH PROJECT, 2023
** Epitech_Zappy
** File description:
** my_write
*/

#include "utility/safe_write.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/select.h>
#include <fcntl.h>
#include <errno.h>
#include "utility/linked_list.h"
#include "utility/strings.h"

list_t **packet_waitlist(void)
{
    static list_t *list = NULL;

    return &list;
}

static void set_socket_blocking(int fd, bool block)
{
    int flags = fcntl(fd, F_GETFL, 0);

    if (flags == -1) {
        perror("fcntl");
        exit(84);
    }
    if (block)
        flags &= ~O_NONBLOCK;
    else
        flags |= O_NONBLOCK;
    if (fcntl(fd, F_SETFL, flags) == -1) {
        perror("fcntl");
        exit(84);
    }
}

static void *try_write(int fd, const void *data, size_t size)
{
    ssize_t res;

    set_socket_blocking(fd, false);
    res = write(fd, data, size);
    set_socket_blocking(fd, true);
    if (res < 0 && errno != EAGAIN && errno != EWOULDBLOCK) {
        perror("write");
        return NULL;
    }
    res = NULL_IF_NEG(res);
    if ((size_t)res != size)
        return create_packet(fd, data + res, size - res);
    return NULL;
}

void write_packets_for_fds(fd_set *fds)
{
    list_t *list = *packet_waitlist();
    list_t *packets_to_send = NULL;
    packet_t *packet;
    packet_t *tmp;

    if (!list)
        return;
    do {
        tmp = NULL;
        packet = list->data;
        if (FD_ISSET(packet->fd, fds))
            tmp = try_write(packet->fd, packet->data, packet->size);
        if (tmp)
            append_node(&packets_to_send, tmp);
        list = list->next;
    } while (list != *packet_waitlist());
    remove_if(packet_waitlist(), fds, compare_packet, free_packet);
    append_list(packet_waitlist(), packets_to_send, true, NULL);
}

void safe_write(int fd, void *data, size_t size)
{
    fd_set set;
    struct timeval timeout = {0, 0};
    list_t *tmp;
    int select_rval;

    FD_ZERO(&set);
    FD_SET(fd, &set);
    select_rval = try_select(fd + 1, NULL, &set, &timeout);
    if (select_rval == 0)
        return append_node(packet_waitlist(), create_packet(fd, data, size));
    tmp = try_write(fd, data, size);
    if (tmp)
        append_node(packet_waitlist(), tmp);
}
