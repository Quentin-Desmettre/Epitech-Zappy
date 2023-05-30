/*
** EPITECH PROJECT, 2023
** Epitech_Zappy
** File description:
** my_write
*/

#include "safe_write.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/select.h>
#include "linked_list.h"
#include "strings.h"

list_t **packet_waitlist(void)
{
    static list_t *list = NULL;

    return &list;
}

static void *my_write(int fd, const void *data, size_t size)
{
    ssize_t res = write(fd, data, size);

    if (res == -1) {
        perror("write");
        exit(84);
    }
    if (res != (ssize_t)size)
        return create_packet(fd, data + res, size - res);
    return NULL;
}

void write_packets_for_fd(int fd)
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
        if (packet->fd == fd)
            tmp = my_write(fd, packet->data, packet->size);
        if (tmp)
            append_node(&packets_to_send, tmp);
        list = list->next;
    } while (list != *packet_waitlist());
    remove_if(packet_waitlist(), &fd, compare_packet, free_packet);
    append_list(packet_waitlist(), packets_to_send, true);
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
            tmp = my_write(packet->fd, packet->data, packet->size);
        if (tmp)
            append_node(&packets_to_send, tmp);
        list = list->next;
    } while (list != *packet_waitlist());
    remove_if(packet_waitlist(), fds, compare_packet, free_packet);
    append_list(packet_waitlist(), packets_to_send, true);
}

void safe_write(int fd, void *data, size_t size)
{
    fd_set set;
    struct timeval timeout = {0, 0};
    list_t *tmp;
    int select_rval;

    FD_ZERO(&set);
    FD_SET(fd, &set);
    select_rval = select(fd + 1, NULL, &set, NULL, &timeout);
    if (select_rval == -1) {
        perror("select");
        exit(84);
    }
    if (select_rval == 0)
        return append_node(packet_waitlist(), create_packet(fd, data, size));
    tmp = my_write(fd, data, size);
    if (tmp)
        append_node(packet_waitlist(), tmp);
}
