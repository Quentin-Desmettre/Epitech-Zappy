/*
** EPITECH PROJECT, 2023
** Epitech_Zappy
** File description:
** handlers3
*/

#include "trantor.h"
#include "server.h"
#include "utility/strings.h"

void notify_incant_start(map_tile_t *tile,
    player_t *base_player, server_t *server)
{
    player_t *player = tile->players->data;
    char *message = my_asprintf("pic %d %d %d", tile->x,
        tile->y, player->level);
    list_t *players = tile->players;
    size_t len = strlen(message);

    do {
        player = players->data;
        if (player->level == base_player->level && !player->is_freezed)
            str_append_free(&message, &len, my_asprintf(" %d", player->id));
        players = players->next;
    } while (players != tile->players);
    send_to_gui(server, message, true);
}

ai_cmd_response_t ai_incantation_start_handler(UNUSED action_t *action,
    server_t *server, player_t *player)
{
    static int incant_id = 1;
    map_tile_t *tile =
        get_tile_by_pos(server->trantor->map, player->x, player->y);

    notify_incant_start(tile, player, server);
    player->incant_id = incant_id++;
    if (!can_level_up(player, tile, false))
        return AI_CMD_RESPONSE_TEXT(my_strdup("ko"));
    freeze_players(tile, player);
    send_to_elevated_clients(server, "Elevation underway\n", player);
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
        answer = my_strdup("ko");
    } else {
        do_level_up(tile, player, server);
        notify_gui(server, END_INCANTATION, player->x, player->y, 1);
        answer = my_asprintf("Current level: %d", player->level);
    }
    send_to_elevated_clients(server, answer, player);
    unfreeze_players(server, tile, player);
    return AI_CMD_RESPONSE_TEXT(answer);
}
