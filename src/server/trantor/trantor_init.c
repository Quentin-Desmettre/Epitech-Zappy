/*
** EPITECH PROJECT, 2023
** Epitech_Zappy
** File description:
** trantor_init
*/

#include "trantor.h"
#include <stdlib.h>
#include "utility/garbage_collector.h"

static map_t *init_map(int width, int height)
{
    map_t *map = my_calloc(1, sizeof(map_t));
    return map;
}

trantor_t *init_trantor(int width, int height)
{
    trantor_t *trantor = my_calloc(sizeof(trantor_t), 1);

    trantor->map = init_map(width, height);
    return trantor;
}

static void destroy_teams(list_t **teams)
{
    free_list(teams, NULL);
}

static void destroy_map(map_t *map)
{
    my_free(map);
}

void destroy_trantor(trantor_t *trantor)
{
    destroy_teams(&trantor->teams);
    destroy_map(trantor->map);
    my_free(trantor);
}
