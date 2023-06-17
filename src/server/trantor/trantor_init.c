/*
** EPITECH PROJECT, 2023
** Epitech_Zappy
** File description:
** trantor_init
*/

#include "trantor.h"
#include "server.h"
#include <stdlib.h>
#include <stdio.h>
#include "utility/garbage_collector.h"

static dim_list_t *init_map(int width, int height)
{
    dim_list_t *map = NULL;
    dim_list_t *sub_list = NULL;

    for (int i = 0; i < height; i++) {
        sub_list = NULL;
        for (int j = 0; j < width; j++)
            dim_append_node(&sub_list, init_tile(j, i));
        dim_append_node(&map, sub_list);
    }
    link_layers(map);
    return map;
}

trantor_t *init_trantor(int width, int height,
    char **team_names, int max_players)
{
    trantor_t *trantor = my_calloc(sizeof(trantor_t), 1);

    trantor->map = init_map(width, height);
    trantor->width = width;
    trantor->height = height;
    for (int i = 0; team_names[i]; i++)
        append_node(&trantor->teams, create_team(team_names[i], max_players));
    return trantor;
}

static void destroy_map(dim_list_t *map)
{
    my_free(map);
}

void destroy_trantor(trantor_t *trantor)
{
    free_list(&trantor->teams, destroy_team);
    destroy_map(trantor->map);
    my_free(trantor);
}
