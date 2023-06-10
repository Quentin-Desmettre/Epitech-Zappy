/*
** EPITECH PROJECT, 2023
** Epitech_Zappy
** File description:
** player
*/

#include "trantor.h"
#include "utility/garbage_collector.h"

player_t *create_player(trantor_t *trantor, team_t *team,
                        const char *team_name)
{
    static int player_id = 0;
    player_t *p = my_calloc(sizeof(player_t), 1);

    p->inventory[FOOD] = 10;
    p->team = team;
    p->team_name = my_strdup(team_name);
    p->id = player_id++;
    return p;
}

void destroy_player(trantor_t *trantor, player_t *player)
{
}
