/*
** EPITECH PROJECT, 2023
** Epitech_Zappy
** File description:
** handlers1
*/

#include "trantor.h"
#include "server.h"

ai_cmd_response_t ai_forward_handler(action_t *action UNUSED,
    trantor_t *trantor, player_t *player)
{
    map_tile_t *tile = get_tile_by_pos(trantor->map, player->x, player->y);

    remove_if(&tile->players, player, NULL, NULL);
    if (player->dir == NORTH)
        player->y = (player->y - 1) % dim_list_size(trantor->map, VERTICAL);
    else if (player->dir == SOUTH)
        player->y = (player->y + 1) % dim_list_size(trantor->map, VERTICAL);
    else if (player->dir == EAST)
        player->x = (player->x + 1) % dim_list_size(trantor->map, HORIZONTAL);
    else if (player->dir == WEST)
        player->x = (player->x - 1) % dim_list_size(trantor->map, HORIZONTAL);
    tile = get_tile_by_pos(trantor->map, player->x, player->y);
    append_node(&tile->players, player);
    return AI_CMD_RESPONSE_OK;
}

ai_cmd_response_t ai_right_handler(action_t *action UNUSED,
    trantor_t *trantor UNUSED, player_t *player)
{
    player->dir = (player->dir + 1) % 4;
    return AI_CMD_RESPONSE_OK;
}

ai_cmd_response_t ai_left_handler(action_t *action UNUSED,
    trantor_t *trantor UNUSED, player_t *player)
{
    player->dir = (player->dir - 1) % 4;
    return AI_CMD_RESPONSE_OK;
}

ai_cmd_response_t ai_look_handler(action_t *action UNUSED,
    trantor_t *trantor, player_t *player)
{
    char *response = malloc(2);
    char *tmp = NULL;

    memset(response, 0, 2);
    sprintf(response, "[");
    for (int i = 0; i != get_nb_tile(player->level); i++) {
        tmp = get_tile_content(select_tile_for_look_command(trantor, player, i));
        if (tmp != NULL) {
            response = realloc(response, strlen(response) + strlen(tmp) + 2);
            sprintf(response, "%s%s", response, tmp);
        }
        if (i != get_nb_tile(player->level) - 1) {
            response = realloc(response, strlen(response) + 2);
            sprintf(response, "%s,", response);
        }
    }
    response = my_realloc(response, strlen(response) + 2);
    sprintf(response, "%s]", response);
    printf("%s\n", response);
    return AI_CMD_RESPONSE_TEXT(response);
}

ai_cmd_response_t ai_inventory_handler(action_t *action,
                                    trantor_t *trantor, player_t *player)
{
}
