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

static struct timespec *
        get_first_action_timeout(action_t *actions[], int nb_actions)
{
    client_t *cli;

    for (int i = 0; i < nb_actions; i++) {
        cli = actions[i]->cli;
        if (cli->data->is_freezed)
            continue;
        return &actions[i]->end_time;
    }
    return NULL;
}

static void fetch_timeout(server_t *server, struct timeval *timeout)
{
    struct timespec now;
    struct timespec infinity = {INT64_MAX, 0};
    struct timespec *ends[] = {
        server->food_timeouts ?
            &((food_timeout_t *)server->food_timeouts->data)->end : NULL,
        get_first_action_timeout(server->actions, server->action_count),
        &server->next_spawn
    };
    const size_t possible_ends = sizeof(ends) / sizeof(ends[0]);
    struct timespec *tmp = &infinity;

    for (size_t i = 0; i < possible_ends; i++)
        if (ends[i] && is_timespec_less(ends[i], tmp))
            tmp = ends[i];
    get_time(&now);
    *timeout = timespec_diff(*tmp, now);
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
    server->client_count++;
}

void run_server(server_t *server)
{
    fd_set readfds;
    fd_set writefds;
    struct timeval timeout = {0, 0};
    int select_rval;
    while (server->run) {
        init_write_set(&writefds);
        fetch_timeout(server, &timeout);
        readfds = server->read_fds;
        select_rval = try_select(FD_SETSIZE, &readfds, &writefds, &timeout);
        if (select_rval == 0) {
            handle_actions(server);
            check_food(server);
            check_resource_spawn(server);
            continue;
        }
        if (FD_ISSET(server->fd, &readfds))
            accept_client(server);
        handle_clients(server, &readfds);
        write_packets_for_fds(&writefds);
    }
}
