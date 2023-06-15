/*
** EPITECH PROJECT, 2023
** Epitech-Zappy
** File description:
** client
*/

#include "server.h"
#include <unistd.h>

bool cmp_cli_food(void *cli, void *food)
{
    return cli == ((food_timeout_t *)food)->cli;
}

void delete_ai_from_server(server_t *server, client_t *cli,
                            bool has_disconnect)
{
    player_t *player = cli->data;

    if (!has_disconnect)
        safe_write(cli->fd, "dead\n", 5);
    player->team->available_slots += (player->is_from_egg ? 0 : 1);
    for (int i = 0; i < server->action_count; i++) {
        if (server->actions[i]->cli != cli)
            continue;
        memmove(&server->actions[i], &server->actions[i + 1],
                sizeof(action_t *) * (server->action_count - i - 1));
        server->action_count--;
        i--;
    }
    remove_if(&server->food_timeouts, cli, cmp_cli_food, my_free);
    notify_gui(server, PLAYER_DEATH, player->id);
    destroy_player(server->trantor, player);
}

void destroy_client(server_t *server, void *data, bool has_disconnect)
{
    client_t *cli = (client_t *)data;

    if (cli->state == AI) {
        printf("Deleting AI...\n");
        delete_ai_from_server(server, cli, has_disconnect);
    }
    close(cli->fd);
    my_free(cli->buffer);
    my_free(cli);
}

void disconnect_client(server_t *server, client_t *cli, bool has_disconnect)
{
    FD_CLR(cli->fd, &server->read_fds);
    server->client_count--;
    destroy_client(server, cli, has_disconnect);
    remove_if(&server->clients, cli, NULL, NULL);
}
