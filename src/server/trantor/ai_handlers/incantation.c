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
    for (int i = 1; i < NB_RESOURCE; i++)
        if (tile->resources[i] < requirements_for_level[player->level][i])
            return false;
    return true;
}

void notify_incant_start(map_tile_t *tile, player_t *base_player, server_t *server)
{
    player_t *player = tile->players->data;
    char *message = my_asprintf("pic %d %d %d", tile->x,
                                tile->y, player->level);
    list_t *players = tile->players;
    size_t len = strlen(message);

    do {
        player = players->data;
        if (player->level == base_player->level)
            str_append_free(&message, &len, my_asprintf(" %d", player->id));
        players = players->next;
    } while (players != tile->players);
    send_to_gui(server, message, true);
}

ai_cmd_response_t ai_incantation_start_handler(UNUSED action_t *action,
    server_t *server, player_t *player)
{
    map_tile_t *tile =
        get_tile_by_pos(server->trantor->map, player->x, player->y);

    notify_incant_start(tile, player, server);
    if (!can_level_up(player, tile)) {
        freeze_players(tile, player);
        return AI_CMD_RESPONSE_TEXT(my_strdup("ko"));
    }
    freeze_players(tile, player);
    send_to_clients_on_tile(server, "Elevation underway\n", player);
    return AI_CMD_RESPONSE_TEXT(my_strdup("Elevation underway"));
}

static void do_level_up(map_tile_t *tile, player_t *player, server_t *server)
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

ai_cmd_response_t ai_incantation_end_handler(UNUSED action_t *action,
    server_t *server, player_t *player)
{
    char *answer;
    map_tile_t *tile =
        get_tile_by_pos(server->trantor->map, player->x, player->y);

    unfreeze_players(server, tile, player);
    if (!can_level_up(player, tile)) {
        notify_gui(server, END_INCANTATION, player->x, player->y, 0);
        return AI_CMD_RESPONSE_KO;
    }
    do_level_up(tile, player, server);
    notify_gui(server, END_INCANTATION, player->x, player->y, 1);
    answer = my_asprintf("Current level: %d\n", player->level + 1);
    send_to_clients_on_tile(server, answer, player);
    return AI_CMD_RESPONSE_TEXT(answer);
}
