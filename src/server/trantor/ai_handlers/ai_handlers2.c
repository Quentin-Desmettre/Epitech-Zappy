/*
** EPITECH PROJECT, 2023
** Epitech_Zappy
** File description:
** handlers2
*/

#include "trantor.h"
#include "server.h"
#include "utility/strings.h"

ai_cmd_response_t ai_broadcast_handler(action_t *action,
    server_t *server, player_t *player)
{
    int direction;
    client_t *client;
    list_t *list_client = server->clients;
    player_t *player_dest;
    char *msg;

    do {
        client = list_client->data;
        player_dest = client->data;
        direction = find_direction(player, player_dest, server->trantor);
        msg = my_asprintf("message %d, %s\n", direction, action->arg);
        safe_write(client->fd, msg, strlen(msg));
        list_client = list_client->next;
    } while (list_client != server->clients);
    notify_gui(server, BROADCAST, player->id, action->arg);
    return AI_CMD_RESPONSE_OK;
}

ai_cmd_response_t ai_connect_nbr_handler(action_t *action UNUSED,
    server_t *server UNUSED, player_t *player)
{
    char *msg = my_asprintf("%d\n", player->team->available_slots);

    return AI_CMD_RESPONSE_TEXT(msg);
}

// TODO: notify TILE_CONTENT / PLAYER_INVENTORY
ai_cmd_response_t ai_set_handler(action_t *action,
    server_t *server, player_t *player)
{
    return AI_CMD_RESPONSE_OK;
}

// TODO: notify EXPULSION
ai_cmd_response_t ai_eject_handler(action_t *action,
    server_t *server, player_t *player)
{
    int save_direct;
    map_tile_t *tile = get_tile_by_pos(server->trantor->map,
        player->x, player->y);
    list_t *players = tile->players;
    player_t *pl;

    while (list_size(tile->players) > 1) {
        if (players->data != player) {
            pl = players->data;
            save_direct = pl->dir;
            pl->dir = player->dir;
            ai_forward_handler(NULL, server, pl);
            pl->dir = save_direct;
            players = tile->players;
        } else
            players = players->next;
    }
    return AI_CMD_RESPONSE_OK;
}

// TODO: notify TILE_CONTENT / PLAYER_INVENTORY
ai_cmd_response_t ai_take_handler(action_t *action,
    server_t *server, player_t *player)
{
    return AI_CMD_RESPONSE_OK;
}
