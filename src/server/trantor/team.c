/*
** EPITECH PROJECT, 2023
** Epitech-Zappy
** File description:
** team
*/

#include "trantor.h"
#include "utility/garbage_collector.h"

team_t *create_team(const char *name, int max_players, int width, int height)
{
    team_t *team = my_calloc(sizeof(team_t), 1);

    team->name = my_strdup(name);
    for (int i = 0; i < max_players; i++)
        create_egg(random() % width, random() % height, false, team);
    return team;
}

void destroy_team(void *t)
{
    team_t *team = t;

    my_free(team->name);
    free_list(&team->players, NULL);
    free_list(&team->eggs, my_free);
    my_free(team);
}
