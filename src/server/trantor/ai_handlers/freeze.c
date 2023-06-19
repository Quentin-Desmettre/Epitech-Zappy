/*
** EPITECH PROJECT, 2023
** Epitech-Zappy
** File description:
** freeze
*/

#include "server.h"
#include "utility/strings.h"

static void unfreeze_player(player_t *player, int incant_id)
{
    struct timespec now;

    if (!player->is_freezed || player->incant_id != incant_id)
        return;
    get_time(&now);
    player->is_freezed = false;
    player->current_action->end_time = timespec_add(now, player->time_left);
}

void freeze_player(player_t *player, int incant_id)
{
    struct timespec now;

    if (player->is_freezed)
        return;
    get_time(&now);
    player->is_freezed = true;
    player->time_left = timespec_diff(player->current_action->end_time, now);
    player->incant_id = incant_id;
}

void freeze_players(map_tile_t *tile, player_t *player)
{
    list_t *players = tile->players;
    player_t *pl;

    do {
        if (!players)
            break;
        pl = players->data;
        if (pl != player && pl->level == player->level)
            freeze_player(pl, player->incant_id);
        players = players->next;
    } while (players != tile->players);
}

void sort_actions(server_t *server)
{
    qsort(server->actions, server->action_count,
        sizeof(action_t *), &action_cmp);
}

void unfreeze_players(server_t *server,
    map_tile_t *tile, player_t *player)
{
    list_t *players = tile->players;
    player_t *pl;

    do {
        if (!players)
            break;
        pl = players->data;
        if (pl != player && pl->level == player->level)
            unfreeze_player(players->data, player->incant_id);
        players = players->next;
    } while (players != tile->players);
    sort_actions(server);
}
