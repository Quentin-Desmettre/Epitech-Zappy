/*
** EPITECH PROJECT, 2023
** Epitech_Zappy
** File description:
** tile_handling
*/

#include "trantor.h"
#include <stdlib.h>
#include "utility/garbage_collector.h"

map_tile_t *init_tile(int x, int y)
{
    map_tile_t *tile = my_calloc(sizeof(map_tile_t), 1);

    tile->players = NULL;
    tile->resources = NULL;
    tile->eggs = NULL;
    tile->x = x;
    tile->y = y;
    return tile;
}

void link_two_vertical_nodes(dim_list_t *node1, dim_list_t *node2)
{
    node2->over = node1->over;
    node2->under = node1;
    node1->over = node2;
}

void link_layers(dim_list_t *map)
{
    dim_list_t *sub_list1 = map;
    dim_list_t *data1 = sub_list1->data;
    dim_list_t *data2;

    for (int i = 0; i != dim_list_size(map, HORIZONTAL); i++) {
        data1->over = data1;
        data1->under = data1;
        data1 = data1->next;
    }
    while (sub_list1->next != map) {
        data1 = sub_list1->data;
        data2 = sub_list1->next->data;
        for (int i = 0; i != dim_list_size(map, HORIZONTAL); i++) {
            link_two_vertical_nodes(data1, data2);
            data1 = data1->next;
            data2 = data2->next;
        }
        sub_list1 = sub_list1->next;
    }
}

map_tile_t *get_tile_by_pos(dim_list_t *map, int x, int y)
{
    dim_list_t *sub_list = map;

    if (y >= 0)
        for (int i = 0; i < y; i++)
            sub_list = sub_list->next;
    else
        for (int i = 0; i > y; i--)
            sub_list = sub_list->prev;
    sub_list = sub_list->data;
    if (x >= 0)
        for (int i = 0; i < x; i++)
            sub_list = sub_list->next;
    else
        for (int i = 0; i > x; i--)
            sub_list = sub_list->prev;
    map_tile_t *tile = sub_list->data;
    return sub_list->data;
}

char *get_tile_content(map_tile_t *tile)
{
    char *content = NULL;
    char *tmp = NULL;
    list_t *tmp_list = tile->players;

    printf("tile->x = %d, tile->y = %d\n", tile->x, tile->y);
    tmp = get_list_players(tmp_list);
    if (tmp != NULL) {
        content = my_realloc(content, strlen(tmp) + 1);
        sprintf(content, "%s", tmp);
    }
    tmp_list = tile->resources;
    tmp = get_list_ressources(tmp_list);
    if (tmp != NULL) {
        content = my_realloc(content, strlen(content) + strlen(tmp) + 1);
        sprintf(content, "%s%s", content, tmp);
    }
    return content;
}

