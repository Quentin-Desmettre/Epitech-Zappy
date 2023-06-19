/*
** EPITECH PROJECT, 2023
** Epitech_Zappy
** File description:
** handlers1
*/

#include "trantor.h"
#include "server.h"
#include "utility/strings.h"

ai_cmd_response_t ai_forward_handler(action_t *action UNUSED,
    server_t *server, player_t *player)
{
    map_tile_t *tile = get_tile_by_pos(server->trantor->map,
        player->x, player->y);

    remove_if(&tile->players, player, NULL, NULL);
    if (player->dir == NORTH)
        player->y--;
    if (player->dir == SOUTH)
        player->y++;
    if (player->dir == EAST)
        player->x++;
    if (player->dir == WEST)
        player->x--;
    tile = get_tile_by_pos(server->trantor->map, player->x, player->y);
    player->x = tile->x;
    player->y = tile->y;
    append_node(&tile->players, player);
    notify_gui(server, PLAYER_POS, player->id,
        player->x, player->y, player->dir);
    return AI_CMD_RESPONSE_OK;
}

ai_cmd_response_t ai_right_handler(action_t *action UNUSED,
    server_t *server UNUSED, player_t *player)
{
    player->dir = (player->dir + 1) % 4;
    notify_gui(server, PLAYER_POS, player->id,
        player->x, player->y, player->dir);
    return AI_CMD_RESPONSE_OK;
}

ai_cmd_response_t ai_left_handler(action_t *action UNUSED,
    server_t *server UNUSED, player_t *player)
{
    if (player->dir == NORTH)
        player->dir = WEST;
    else
        player->dir = (player->dir - 1) % 4;
    notify_gui(server, PLAYER_POS, player->id,
        player->x, player->y, player->dir);
    return AI_CMD_RESPONSE_OK;
}

ai_cmd_response_t ai_look_handler(action_t *action UNUSED,
    server_t *server, player_t *player)
{
    char *response = my_strdup("[");
    char *tile_content = NULL;
    size_t len;

    for (int i = 0; i != get_nb_tile(player->level); i++) {
        tile_content = get_tile_content(
            select_tile_for_look_command(server->trantor, player, i));
        response = str_concat_free(&len, 2, response, tile_content);
        if (i != get_nb_tile(player->level) - 1)
            str_append(&response, ", ");
    }
    str_append(&response, "]");
    return AI_CMD_RESPONSE_TEXT(response);
}

ai_cmd_response_t ai_inventory_handler(action_t *action UNUSED,
    server_t *server UNUSED, player_t *player)
{
    resource_t *inventory = player->inventory;
    char *response = my_asprintf("[food %d, linemate %d, deraumere %d, "
        "sibur %d, mendiane %d, phiras %d, thystame %d]",
        inventory[FOOD], inventory[LINEMATE], inventory[DERAUMERE],
        inventory[SIBUR], inventory[MENDIANE], inventory[PHIRAS],
        inventory[THYSTAME]);

    return AI_CMD_RESPONSE_TEXT(response);
}
