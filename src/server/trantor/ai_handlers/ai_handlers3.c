/*
** EPITECH PROJECT, 2023
** Epitech_Zappy
** File description:
** handlers3
*/

#include "trantor.h"
#include "server.h"
#include "utility/strings.h"

void send_message(const action_t *action, server_t *server, player_t *player,
    const client_t *client)
{
    char *msg;
    player_t *player_dest = client->data;
    int direction = find_direction(player, player_dest, server->trantor);

    msg = my_asprintf("message %d, %s\n", direction, action->arg);
    safe_write_free(client->fd, msg);
}

