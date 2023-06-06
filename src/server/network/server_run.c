/*
** EPITECH PROJECT, 2023
** Epitech_Zappy
** File description:
** server_run
*/

#include "server.h"
#include <stdio.h>
#include <sys/socket.h>
#include "utility/strings.h"
#include <string.h>

static void init_read_set(server_t *server)
{
    list_t *tmp = server->clients;

    FD_ZERO(&server->read_fds);
    FD_SET(server->fd, &server->read_fds);
    if (!tmp)
        return;
    do {
        FD_SET(((client_t *)tmp->data)->fd, &server->read_fds);
        tmp = tmp->next;
    } while (tmp != server->clients);
}

static void init_write_set(fd_set *write_fds)
{
    list_t *tmp = *packet_waitlist();

    FD_ZERO(write_fds);
    if (!tmp)
        return;
    do {
        FD_SET(((packet_t *)tmp->data)->fd, write_fds);
        tmp = tmp->next;
    } while (tmp != *packet_waitlist());
}

static void fetch_timeout(server_t *server, struct timeval *timeout)
{
    action_t *action;
    struct timespec now;

    if (!server->action_count) {
        *timeout = (struct timeval) {.tv_sec = INT64_MAX, .tv_usec = 0};
        return;
    }
    action = server->actions[0];
    get_time(&now);
    timeout->tv_sec = action->end_time.tv_sec - now.tv_sec;
    timeout->tv_usec = action->end_time.tv_nsec / 1000 - now.tv_nsec / 1000;
}

static void accept_client(server_t *server)
{
    client_t *cli = my_calloc(1, sizeof(client_t));

    cli->fd = accept(server->fd, NULL, NULL);
    if (cli->fd == -1) {
        perror("accept");
        exit(84);
    }
    cli->state = CONNECTED;
    append_node(&server->clients, cli);
    safe_write(cli->fd, WELCOME_MESSAGE, strlen(WELCOME_MESSAGE));
    FD_SET(cli->fd, &server->read_fds);
}

void run_server(server_t *server)
{
    fd_set readfds;
    fd_set writefds;
    struct timeval timeout = {0, 0};
    int select_rval;

    init_read_set(server);
    while (server->run) {
        init_write_set(&writefds);
        fetch_timeout(server, &timeout);
        readfds = server->read_fds;
        select_rval = try_select(FD_SETSIZE, &readfds, &writefds, &timeout);
        if (select_rval == 0) {
            handle_actions(server);
            continue;
        }
        if (FD_ISSET(server->fd, &readfds))
            accept_client(server);
        handle_clients(server, &readfds);
        write_packets_for_fds(&writefds);
    }
}
