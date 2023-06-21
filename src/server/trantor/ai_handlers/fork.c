/*
** EPITECH PROJECT, 2023
** Epitech-Zappy
** File description:
** fork
*/

#include "server.h"

egg_t *create_egg(int x, int y, bool is_forked, team_t *team)
{
    static int egg_id = 0;
    egg_t *egg = my_calloc(sizeof(egg_t), 1);

    egg->x = x;
    egg->y = y;
    egg->is_forked = is_forked;
    egg->id = egg_id;
    egg->team = team;
    egg_id++;
    append_node(&team->eggs, egg);
    return egg;
}

ai_cmd_response_t ai_fork_handler(UNUSED action_t *action,
    server_t *server, player_t *player)
{
    egg_t *egg = create_egg(player->x, player->y, true, player->team);

    egg->player_id = player->id;
    notify_gui(server, EGG_READY, egg->id, player->id, egg->x, egg->y);
    return AI_CMD_RESPONSE_OK;
}

ai_cmd_response_t ai_fork_pre_check(UNUSED action_t *action,
    server_t *server, player_t *player)
{
    notify_gui(server, EGG_PLANTED, player->id);
    return AI_CMD_RESPONSE_TEXT(my_strdup(""));
}
