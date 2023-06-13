/*
** EPITECH PROJECT, 2023
** Epitech-Zappy
** File description:
** freeze
*/

#include "server.h"
#include "utility/strings.h"

void unfreeze_player(player_t *player)
{
    struct timespec now;

    get_time(&now);
    player->is_freezed = false;
    player->current_action->end_time = timespec_add(now, player->time_left);
}

void freeze_player(player_t *player)
{
    struct timespec now;

    if (player->is_freezed)
        return;
    get_time(&now);
    player->is_freezed = true;
    player->time_left = timespec_diff(player->current_action->end_time, now);
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
            freeze_player(players->data);
        players = players->next;
    } while (players != tile->players);
}

void unfreeze_players(server_t *server, map_tile_t *tile, player_t *player)
{
    list_t *players = tile->players;
    player_t *pl;

    do {
        if (!players)
            break;
        pl = players->data;
        if (pl != player && pl->level == player->level)
            unfreeze_player(players->data);
        players = players->next;
    } while (players != tile->players);
    qsort(server->actions, server->action_count,
        sizeof(action_t *), &action_cmp);
}
