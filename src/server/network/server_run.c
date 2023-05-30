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

void handle_actions(server_t *server)
{
    list_t *tmp = server->clients;
    action_t *action;
    struct timespec now;
    struct timespec end_time;
    client_t *cli;

    get_time(&now);
    do {
        cli = (client_t *)tmp->data;
        action = cli->data->current_action;
        end_time = get_action_end(action, server->params.freq);
        if (end_time.tv_sec <= now.tv_sec ||
        (end_time.tv_sec == now.tv_sec && end_time.tv_nsec <= now.tv_nsec))
            do_action(action, server->trantor, cli);
        tmp = tmp->next;
    } while (tmp != server->clients);
}

static void init_fd_sets(server_t *server, fd_set *read_fds, fd_set *write_fds)
{
    list_t *tmp = server->clients;

    FD_ZERO(read_fds);
    FD_ZERO(write_fds);
    FD_SET(server->fd, read_fds);
    if (tmp) {
        do {
            FD_SET(((client_t *)tmp->data)->fd, read_fds);
            tmp = tmp->next;
        } while (tmp != server->clients);
    }
    tmp = *packet_waitlist();
    if (!tmp)
        return;
    do {
        FD_SET(((packet_t *)tmp->data)->fd, write_fds);
        tmp = tmp->next;
    } while (tmp != *packet_waitlist());
}

static void fetch_timeout(server_t *server, struct timeval *timeout)
{
    list_t *tmp = server->clients;
    action_t *action;
    struct timespec now;
    struct timespec tmp_timeout = {UINT64_MAX, UINT64_MAX};
    struct timespec action_end;

    do {
        action = ((client_t *)tmp->data)->data->current_action;
        action_end = get_action_end(action, server->params.freq);
        if (action_end.tv_sec < tmp_timeout.tv_sec ||
        (action_end.tv_sec == tmp_timeout.tv_sec &&
        action_end.tv_nsec < tmp_timeout.tv_nsec))
            tmp_timeout = action_end;
        tmp = tmp->next;
    } while (tmp != server->clients);
    get_time(&now);
    timeout->tv_sec = tmp_timeout.tv_sec - now.tv_sec;
    timeout->tv_usec = tmp_timeout.tv_nsec / 1000 - now.tv_nsec / 1000;
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
}

void run_server(server_t *server)
{
    fd_set readfds;
    fd_set writefds;
    struct timeval timeout = {0, 0};
    int select_rval;

    while (server->run) {
        init_fd_sets(server, &readfds, &writefds);
        fetch_timeout(server, &timeout);
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
