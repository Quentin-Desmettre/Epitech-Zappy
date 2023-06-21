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
    client_t *client;
    list_t *list_client = server->clients;
    player_t *player_dest;

    do {
        client = list_client->data;
        if (client->state != AI) {
            list_client = list_client->next;
            continue;
        }
        player_dest = client->data;
        if (player_dest == player) {
            list_client = list_client->next;
            continue;
        }
        send_message(action, server, player, client);
        list_client = list_client->next;
    } while (list_client != server->clients);
    notify_gui(server, BROADCAST, player->id, action->arg);
    return AI_CMD_RESPONSE_OK;
}

ai_cmd_response_t ai_set_handler(action_t *action,
    server_t *server, player_t *player)
{
    map_tile_t *tile = get_tile_by_pos(server->trantor->map,
        player->x, player->y);
    int i;

    if (!str_array_contains(ressources_names, (char *)action->arg))
        return AI_CMD_RESPONSE_KO;
    for (i = 0; i < NB_RESOURCE; i++)
        if (!strcmp(ressources_names[i], (char *)action->arg))
            break;
    if (player->inventory[i] <= 0)
        return AI_CMD_RESPONSE_KO;
    notify_gui(server, RESOURCE_DROP, player->id, i);
    player->inventory[i]--;
    tile->resources[i]++;
    return AI_CMD_RESPONSE_OK;
}

void remove_players(server_t *server, map_tile_t *tile, player_t *player)
{
    player_t *pl;
    list_t *players = tile->players;
    int save_direct;

    while (list_size(tile->players) > 1) {
        if (players->data != player) {
            pl = players->data;
            save_direct = pl->dir;
            pl->dir = player->dir;
            ai_forward_handler(NULL, server, pl);
            pl->dir = save_direct;
            players = tile->players;
            notify_gui(server, EXPULSION, pl->id);
        } else
            players = players->next;
    }
}

ai_cmd_response_t ai_eject_handler(action_t *action UNUSED,
    server_t *server, player_t *player)
{
    map_tile_t *tile = get_tile_by_pos(server->trantor->map,
        player->x, player->y);

    remove_players(server, tile, player);
    while (tile->eggs) {
        notify_gui(server, EGG_DEAD, (int)(long)tile->eggs->data);
        remove_node(&tile->eggs, 0, NULL);
    }
    return AI_CMD_RESPONSE_OK;
}

ai_cmd_response_t ai_take_handler(action_t *action,
    server_t *server, player_t *player)
{
    map_tile_t *tile = get_tile_by_pos(server->trantor->map,
        player->x, player->y);
    int i;

    if (!str_array_contains(ressources_names, (char *)action->arg))
        return AI_CMD_RESPONSE_KO;
    for (i = 0; i < NB_RESOURCE; i++)
        if (!strcmp(ressources_names[i], (char *)action->arg))
            break;
    if (tile->resources[i] <= 0)
        return AI_CMD_RESPONSE_KO;
    player->inventory[i]++;
    tile->resources[i]--;
    notify_gui(server, RESOURCE_COLLECT, player->id, i);
    return AI_CMD_RESPONSE_OK;
}
