/*
** EPITECH PROJECT, 2023
** Epitech_Zappy
** File description:
** my_write
*/

#ifndef EPITECH_ZAPPY_SAFE_WRITE_H
    #define EPITECH_ZAPPY_SAFE_WRITE_H
    #include <stddef.h>
    #include <stdbool.h>
    #include <sys/select.h>
    #include "linked_list.h"

typedef struct {
    int fd;
    void *data;
    size_t size;
} packet_t;

/**
 * @brief Create a packet object, by duplicating the data
 *
 * @param fd The fd to send the packet to
 * @param data Data to send
 * @param size Size of the data
 * @return void* The packet
 */
void *create_packet(int fd, const void *data, size_t size);

/**
 * @brief Free a packet
 * @param packet The packet to free
 */
void free_packet(void *packet);

/**
 * @brief Compare a packet to a fd
 * @param fd Pointer to an int containing the fd
 * @param packet The packet to compare
 * @return true If the packet's fd is equal to the fd
 */
bool compare_packet(void *fd, void *packet);

/**
 * @brief Write all packets that have been queued for the given fds
 *
 * This function then removes the packets from the queue.
 * If a packet could not be written in its entirety, only the part that was
 * written is removed from the queue.
 * @param fds The fds to write to
 */
void write_packets_for_fds(fd_set *fds);


/**
 * @brief Write data to a fd, or queue it if the fd is not ready
 * @param fd The fd to write to
 * @param data The data to write
 * @param size The size of the data
 */
void safe_write(int fd, void *data, size_t size);

/**
 * @brief Get the packet waitlist object
 * @return list_t** The packet waitlist
 */
list_t **packet_waitlist(void);

#endif //EPITECH_ZAPPY_SAFE_WRITE_H
