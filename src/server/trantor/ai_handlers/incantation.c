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
    answer = my_asprintf("Current level: %d\n", player->level);
    send_to_clients_on_tile(server, answer, player);
    return AI_CMD_RESPONSE_TEXT(answer);
}
