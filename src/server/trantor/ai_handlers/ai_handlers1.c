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
    map_tile_t *tile = get_tile_by_pos(server->trantor->map, player->x, player->y);

    remove_if(&tile->players, player, NULL, NULL);
    if (player->dir == NORTH)
        player->y = (player->y - 1) % dim_list_size(server->trantor->map, HORIZONTAL);
    if (player->dir == SOUTH)
        player->y = (player->y + 1) % dim_list_size(server->trantor->map, HORIZONTAL);
    if (player->dir == EAST)
        player->x = (player->x + 1) % dim_list_size(server->trantor->map, VERTICAL);
    if (player->dir == WEST)
        player->x = (player->x - 1) % dim_list_size(server->trantor->map, VERTICAL);
    tile = get_tile_by_pos(server->trantor->map, player->x, player->y);
    player->x = tile->x;
    player->y = tile->y;
    append_node(&tile->players, player);
    return AI_CMD_RESPONSE_OK;
}

ai_cmd_response_t ai_right_handler(action_t *action UNUSED,
    server_t *server UNUSED, player_t *player)
{
    player->dir = (player->dir + 1) % 4;
    return AI_CMD_RESPONSE_OK;
}

ai_cmd_response_t ai_left_handler(action_t *action UNUSED,
    server_t *server UNUSED, player_t *player)
{
    player->dir = (player->dir - 1) % 4;
    return AI_CMD_RESPONSE_OK;
}

ai_cmd_response_t ai_look_handler(action_t *action UNUSED,
    server_t *server, player_t *player)
{
    char *response = my_malloc(2);
    char *tmp = NULL;

    memset(response, 0, 2);
    sprintf(response, "[");
    for (int i = 0; i != get_nb_tile(player->level); i++) {
        tmp = get_tile_content(select_tile_for_look_command(server->trantor, player, i));
        if (tmp != NULL) {
            response = my_realloc(response, strlen(response) + strlen(tmp) + 2);
            sprintf(response, "%s%s", response, tmp);
        }
        if (i != get_nb_tile(player->level) - 1) {
            response = my_realloc(response, strlen(response) + 2);
            sprintf(response, "%s,", response);
        }
    }
    response = my_realloc(response, strlen(response) + 2);
    sprintf(response, "%s]", response);
    return AI_CMD_RESPONSE_TEXT(response);
}

ai_cmd_response_t ai_inventory_handler(action_t *action UNUSED,
    server_t *server UNUSED, player_t *player)
{
    char *response = my_asprintf("[food %d, linemate %d, deraumere %d, sibur %d, mendiane %d, phiras %d, thystame %d]",
        player->inventory[FOOD], player->inventory[LINEMATE], player->inventory[DERAUMERE],
        player->inventory[SIBUR], player->inventory[MENDIANE], player->inventory[PHIRAS],
        player->inventory[THYSTAME]);
    return AI_CMD_RESPONSE_TEXT(response);
}
