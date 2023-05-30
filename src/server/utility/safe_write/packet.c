/*
** EPITECH PROJECT, 2023
** Epitech_Zappy
** File description:
** packet
*/

#include "safe_write.h"
#include "strings.h"
#include <stdlib.h>
#include "garbage_collector.h"

void *create_packet(int fd, const void *data, size_t size)
{
    packet_t *packet = my_malloc(sizeof(packet_t));

    packet->fd = fd;
    packet->data = memdup(data, size);
    packet->size = size;
    return packet;
}

void free_packet(void *packet)
{
    my_free(((packet_t *)packet)->data);
    my_free(packet);
}

bool compare_packet(void *fd, void *packet)
{
    int int_fd = *(int *)fd;

    return ((packet_t *)packet)->fd == int_fd;
}
