/*
** EPITECH PROJECT, 2023
** Epitech_Zappy
** File description:
** packet
*/

#include "utility/safe_write.h"
#include "utility/strings.h"
#include "utility/garbage_collector.h"

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

bool compare_packet(void *fds, void *packet)
{
    return FD_ISSET(((packet_t *)(packet))->fd, (fd_set *)(fds));
}
