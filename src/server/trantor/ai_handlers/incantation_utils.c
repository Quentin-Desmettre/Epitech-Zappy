/*
** EPITECH PROJECT, 2023
** Epitech-Zappy
** File description:
** incantation_utils
*/

#include "server.h"
#include "utility/strings.h"

int count_same_level(player_t *player, map_tile_t *tile)
{
    int nb_same_level = 0;
    list_t *players = tile->players;
    player_t *pl;

    do {
        pl = players->data;
        if (pl->level == player->level && !pl->is_freezed)
            nb_same_level++;
        players = players->next;
    } while (players != tile->players);
    return nb_same_level;
}

bool can_level_up(player_t *player, map_tile_t *tile)
{
    int nb_same_level = count_same_level(player, tile);

    if (requirements_for_level[player->level][0] < nb_same_level)
        return false;
    for (int i = 1; i < NB_RESOURCE; i++)
        if (tile->resources[i] < requirements_for_level[player->level][i])
            return false;
    return true;
}

void do_level_up(map_tile_t *tile, player_t *player, server_t *server)
{
    list_t *players = tile->players;
    int base_level = player->level;
    player_t *pl;

    for (int i = 1; i < NB_RESOURCE; i++)
        tile->resources[i] -= requirements_for_level[player->level][i];
    notify_gui(server, TILE_CONTENT, tile->x, tile->y, tile->resources[FOOD],
            tile->resources[LINEMATE], tile->resources[DERAUMERE],
            tile->resources[SIBUR], tile->resources[MENDIANE],
            tile->resources[PHIRAS], tile->resources[THYSTAME]);
    do {
        pl = players->data;
        if (pl->level == base_level) {
            pl->level++;
            notify_gui(server, PLAYER_LVL_UP, pl->id, pl->level);
        }
        players = players->next;
    } while (players != tile->players);
}

ai_cmd_response_t ai_connect_nbr_handler(action_t *action UNUSED,
    server_t *server UNUSED, player_t *player)
{
    char *msg = my_asprintf("%d", list_size(player->team->eggs));

    return AI_CMD_RESPONSE_TEXT(msg);
}
