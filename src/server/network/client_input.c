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
    cli->buffer = my_realloc(cli->buffer, cli->buffer_size + bytes + 1);
    memcpy(cli->buffer + cli->buffer_size, tmp_buf, bytes);
    cli->buffer[cli->buffer_size + bytes] = '\0';
    cli->buffer_size += bytes;
    return true;
}

static void handle_client_input(server_t *server, client_t *cli)
{
    char **args;
    int nb_args;
    if (!fetch_client_data(cli))
        return disconnect_client(server, cli, true);
    if (strchr(cli->buffer, '\n') == NULL)
        return;
    args = split_on(cli->buffer, "\n", &nb_args);
    if (!str_ends_with(cli->buffer, "\n")) {
        my_free(cli->buffer);
        cli->buffer = args[nb_args - 1];
        args[nb_args - 1] = NULL;
        cli->buffer_size = strlen(cli->buffer);
    } else {
        my_free(cli->buffer);
        cli->buffer = NULL;
        cli->buffer_size = 0;
    }
    for (int i = 0; args[i]; i++) {
        if (strlen(args[i])) {
            STATE_HANDLER[cli->state](server, cli, args[i]);
        }
    }
}

void handle_clients(server_t *server, fd_set *read_fds)
{
    list_t *tmp = server->clients;
    list_t *next;
    client_t *cli;
    int nb_to_handle = server->client_count;

    while (nb_to_handle > 0) {
        cli = (client_t *)tmp->data;
        next = tmp->next;
        if (FD_ISSET(cli->fd, read_fds))
            handle_client_input(server, cli);
        tmp = next;
        nb_to_handle--;
    }
}
