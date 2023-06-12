/*
** EPITECH PROJECT, 2023
** Epitech-Zappy
** File description:
** fork
*/

#include "server.h"

int create_egg(team_t *team)
{
    static int egg_id = 0;

    append_node(&team->egg_numbers, (void *)(long)egg_id);
    team->eggs++;
    return egg_id++;
}

ai_cmd_response_t ai_fork_handler(UNUSED action_t *action,
    server_t *server, player_t *player)
{
    int egg_id = create_egg(player->team);

    notify_gui(server, EGG_READY, egg_id, player->id, player->x, player->y);
    return AI_CMD_RESPONSE_OK;
}

ai_cmd_response_t ai_fork_pre_check(UNUSED action_t *action,
    server_t *server, player_t *player)
{
    notify_gui(server, EGG_PLANTED, player->id);
    return AI_CMD_RESPONSE_OK;
}
