/*
** EPITECH PROJECT, 2023
** Epitech-Zappy
** File description:
** freeze
*/

#include "server.h"
#include "utility/strings.h"

static void unfreeze_player(server_t *server, player_t *player, int incant_id)
{
    struct timespec now;

    if (!player->is_freezed || player->incant_id != incant_id)
        return;
    player->is_freezed = false;
    get_time(&now);
    if (!player->current_action) {
        pop_waitlist(server, player->client);
    } else
        player->current_action->end_time = timespec_add(now, player->time_left);
}

void freeze_player(player_t *player, int incant_id)
{
    struct timespec now;

    if (player->is_freezed)
        return;
    get_time(&now);
    player->is_freezed = true;
    player->time_left = (player->current_action ?
        timespec_diff(player->current_action->end_time, now)
        : (struct timeval){0, 0});
    player->incant_id = incant_id;
}

void freeze_players(list_t *players)
{
    player_t *player;
    list_t *tmp = players;

    for (int i = 0, max = list_size(players); i < max; i++, tmp = tmp->next) {
        player = tmp->data;
        freeze_player(player, player->incant_id);
    }
}

void sort_actions(server_t *server)
{
    qsort(server->actions, server->action_count,
        sizeof(action_t *), &action_cmp);
}

void unfreeze_players(server_t *server, list_t *players)
{
    player_t *player;
    list_t *tmp = players;

    for (int i = 0, max = list_size(players); i < max; i++, tmp = tmp->next) {
        player = tmp->data;
        unfreeze_player(server, player, player->incant_id);
    }
}
