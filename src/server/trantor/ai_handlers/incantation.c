/*
** EPITECH PROJECT, 2023
** Epitech_Zappy
** File description:
** handlers3
*/

#include "trantor.h"
#include "server.h"
#include "utility/strings.h"

static int count_same_level(player_t *player, map_tile_t *tile)
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

static bool can_level_up(player_t *player, map_tile_t *tile)
{
    int nb_same_level = count_same_level(player, tile);

    if (requirements_for_level[player->level][0] < nb_same_level)
        return false;
    for (int i = 0; i < NB_RESOURCE; i++) {
        if (tile->resources[i] < requirements_for_level[player->level][i + 1])
            return false;
    }
    return true;
}

ai_cmd_response_t ai_incantation_start_handler(action_t *action,
    server_t *server, player_t *player)
{
    map_tile_t *tile =
        get_tile_by_pos(server->trantor->map, player->x, player->y);

    if (!can_level_up(player, tile)) {
        freeze_players(tile, player);
        return AI_CMD_RESPONSE_TEXT(my_strdup("ko"));
    }
    freeze_players(tile, player);
    send_to_clients_on_tile(server, "Elevation underway\n", player);
    return AI_CMD_RESPONSE_TEXT(my_strdup("Elevation underway"));
}

void do_level_up(map_tile_t *tile, player_t *player)
{
    list_t *players = tile->players;
    int base_level = player->level;
    player_t *pl;

    for (int i = 0; i < NB_RESOURCE; i++)
        tile->resources[i] -= requirements_for_level[player->level][i + 1];
    do {
        pl = players->data;
        if (pl->level == base_level)
            pl->level++;
        players = players->next;
    } while (players != tile->players);
}

ai_cmd_response_t ai_incantation_end_handler(UNUSED action_t *action,
    server_t *server, player_t *player)
{
    char *answer;
    map_tile_t *tile =
        get_tile_by_pos(server->trantor->map, player->x, player->y);

    unfreeze_players(server, tile, player);
    if (!can_level_up(player, tile))
        return AI_CMD_RESPONSE_KO;
    do_level_up(tile, player);
    answer = my_asprintf("Current level: %d\n", player->level + 1);
    send_to_clients_on_tile(server, answer, player);
    return AI_CMD_RESPONSE_TEXT(answer);
}

/*
 * TODO: send events to GUI:
 * - plv
 * - pic
 * - pie
 */
