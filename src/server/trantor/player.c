/*
** EPITECH PROJECT, 2023
** Epitech_Zappy
** File description:
** player
*/

#include "trantor.h"
#include <stdlib.h>
#include "utility/garbage_collector.h"

player_t *create_player(trantor_t *trantor, const char *team_name)
{
    return my_calloc(sizeof(player_t), 1);
}

void destroy_player(trantor_t *trantor, player_t *player)
{
}
