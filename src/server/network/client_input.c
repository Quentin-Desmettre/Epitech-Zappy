/*
** EPITECH PROJECT, 2023
** Epitech_Zappy
** File description:
** client_input
*/

#include "server.h"
#include "utility/strings.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>

const state_handler_t STATE_HANDLER[] = {
        handle_connected,
        handle_gui,
        handle_ai
};

void destroy_client(server_t *server, void *data)
{
    client_t *cli = (client_t *)data;

    close(cli->fd);
    if (cli->state == AI)
        destroy_player(server->trantor, cli->data);
    my_free(cli->buffer);
    my_free(cli);
}

static void disconnect_client(server_t *server, client_t *cli)
{
    destroy_client(server, cli);
    remove_if(&server->clients, cli, NULL, NULL);
}

static bool fetch_client_data(client_t *cli)
{
    int bytes = bytes_available(cli->fd);
    char *tmp_buf;

    if (bytes == 0)
        return false;
    tmp_buf = my_calloc(bytes, sizeof(char));
    if (read(cli->fd, tmp_buf, bytes) == -1) {
        perror("read");
        exit(84);
    }
    cli->buffer = my_realloc(cli->buffer, cli->buffer_size + bytes);
    memcpy(cli->buffer + cli->buffer_size, tmp_buf, bytes);
    cli->buffer_size += bytes;
    return true;
}

static void handle_client_input(server_t *server, client_t *cli)
{
    char **args;
    int nb_args;

    if (!fetch_client_data(cli))
        return disconnect_client(server, cli);
    if (strchr(cli->buffer, '\n') == NULL)
        return;
    args = split_on(cli->buffer, "\n", &nb_args);
    free(cli->buffer);
    cli->buffer = NULL;
    cli->buffer_size = 0;
    if (!str_ends_with(cli->buffer, "\n")) {
        cli->buffer = args[nb_args - 1];
        args[nb_args - 1] = NULL;
        cli->buffer_size = strlen(cli->buffer);
    }
    for (int i = 0; args[i]; i++)
        STATE_HANDLER[cli->state](server, cli, args[i]);
}

void handle_clients(server_t *server, fd_set *read_fds)
{
    list_t *tmp = server->clients;
    client_t *cli;

    do {
        cli = tmp->data;
        if (FD_ISSET(cli->fd, read_fds))
            handle_client_input(server, cli);
        tmp = tmp->next;
    } while (tmp != server->clients);
}
