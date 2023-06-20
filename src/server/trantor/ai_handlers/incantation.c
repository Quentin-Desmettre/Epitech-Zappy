/*
** EPITECH PROJECT, 2023
** Epitech_Zappy
** File description:
** handlers3
*/

#include "trantor.h"
#include "server.h"
#include "utility/strings.h"

void notify_incant_start(player_t *base_player, list_t *players,
    server_t *server)
{
    player_t *player;
    char *message = my_asprintf("pic %d %d %d %d", base_player->x,
        base_player->y, base_player->level, base_player->id);
    list_t *tmp = players;
    size_t len = strlen(message);

    for (int i = 0, max = list_size(players); i < max;
    i++, tmp = tmp->next) {
        player = tmp->data;
        str_append_free(&message, &len, my_asprintf(" %d", player->id));
    }
    str_append(&message, "\n");
    send_to_gui(server, message, true);
}

list_t *set_incant_id(list_t *players, player_t *base)
{
    player_t *player;
    list_t *tmp = players;
    list_t *related_players = NULL;

    for (int i = 0, max = list_size(players); i < max;
    i++, tmp = tmp->next) {
        player = tmp->data;
        if (player != base && player->level == base->level
        && !player->is_freezed) {
            player->incant_id = base->incant_id;
            append_node(&related_players, player);
        }
    }
    return related_players;
}

void send_to_players(list_t *players, char *msg)
{
    player_t *player;
    list_t *tmp = players;
    size_t msg_len = strlen(msg);

    for (int i = 0, max = list_size(players); i < max;
    i++, tmp = tmp->next) {
        player = tmp->data;
        safe_write(((client_t *)player->client)->fd, msg, msg_len);
    }
}

ai_cmd_response_t ai_incantation_start_handler(UNUSED action_t *action,
    server_t *server, player_t *player)
{
    static int incant_id = 1;
    map_tile_t *tile =
        get_tile_by_pos(server->trantor->map, player->x, player->y);
    list_t *related_players = NULL;

    player->incant_id = incant_id++;
    player->related_players = NULL;
    if (!can_level_up(player, tile, false)) {
        notify_incant_start(player, NULL, server);
        return AI_CMD_RESPONSE_TEXT(my_strdup("ko"));
    }
    related_players = set_incant_id(tile->players, player);
    player->related_players = related_players;
    freeze_players(related_players);
    notify_incant_start(player, related_players, server);
    send_to_players(related_players, "Elevation underway\n");
    return AI_CMD_RESPONSE_TEXT(my_strdup("Elevation underway"));
}

ai_cmd_response_t ai_incantation_end_handler(UNUSED action_t *action,
    server_t *server, player_t *player)
{
    char *answer;
    map_tile_t *tile =
        get_tile_by_pos(server->trantor->map, player->x, player->y);

    if (!can_level_up(player, tile, true)) {
        notify_gui(server, END_INCANTATION, player->x, player->y, 0);
        answer = my_strdup("ko\n");
    } else {
        do_level_up(tile, player, server);
        notify_gui(server, END_INCANTATION, player->x, player->y, 1);
        answer = my_asprintf("Current level: %d\n", player->level);
    }
    send_to_players(player->related_players, answer);
    unfreeze_players(server, player->related_players);
    answer[strlen(answer) - 1] = '\0';
    free_list(&player->related_players, NULL);
    return AI_CMD_RESPONSE_TEXT(answer);
}
