/*
** EPITECH PROJECT, 2023
** Epitech-Zappy
** File description:
** team
*/

#include "trantor.h"
#include "utility/garbage_collector.h"

team_t *create_team(const char *name, int max_players)
{
    team_t *team = my_calloc(sizeof(team_t), 1);

    team->name = my_strdup(name);
    team->available_slots = max_players;
    return team;
}
