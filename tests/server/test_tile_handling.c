/*
** EPITECH PROJECT, 2023
** Epitech-Zappy
** File description:
** test_handle_actions
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "server.h"
#include "utility/strings.h"

void link_two_vertical_nodes(dim_list_t *node1, dim_list_t *node2);

Test(init_tile, init_a_list)
{
    map_tile_t *tile = init_tile(1, 2);

    cr_assert(tile != NULL);
    my_free(tile);
}

Test(link_two_vertical_nodes, link_two_nodes)
{
    dim_list_t *node1 = my_calloc(sizeof(dim_list_t), 1);
    dim_list_t *node2 = my_calloc(sizeof(dim_list_t), 1);

    node1->over = (void *)0xdeadbeef;
    link_two_vertical_nodes(node1, node2);
    cr_assert_eq(node2->over, 0xdeadbeef);
    cr_assert_eq(node2->under, node1);
    cr_assert_eq(node1->over, node2);
    my_free(node1);
    my_free(node2);
}

Test(get_tile_by_pos, get_tile_at_1_1)
{
    dim_list_t *map = my_calloc(sizeof(dim_list_t), 1);
    dim_list_t *list = my_calloc(sizeof(dim_list_t), 1);
    dim_list_t *tile = my_calloc(sizeof(dim_list_t), 1);
    dim_list_t *tile2 = my_calloc(sizeof(dim_list_t), 1);

    map->next = list;
    list->data = tile;
    tile->next = tile2;
    tile2->data = (void *)0xdeadbeef;
    cr_assert_eq(get_tile_by_pos(map, 1, 1), 0xdeadbeef);
    my_free(map);
    my_free(list);
    my_free(tile);
    my_free(tile2);
}

Test(get_tile_by_pos, get_tile_at_1_1_negative)
{
    dim_list_t *map = my_calloc(sizeof(dim_list_t), 1);
    dim_list_t *list = my_calloc(sizeof(dim_list_t), 1);
    dim_list_t *tile = my_calloc(sizeof(dim_list_t), 1);
    dim_list_t *tile2 = my_calloc(sizeof(dim_list_t), 1);

    map->prev = list;
    list->data = tile;
    tile->prev = tile2;
    tile2->data = (void *)0xdeadbeef;
    cr_assert_eq(get_tile_by_pos(map, -1, -1), 0xdeadbeef);
    my_free(map);
    my_free(list);
    my_free(tile);
    my_free(tile2);
}

Test(get_tile_content, get_empty_tile_content)
{
    map_tile_t *tile = my_calloc(sizeof(map_tile_t), 1);

    tile->players = NULL;
    tile->eggs = NULL;
    tile->x = 1;
    tile->y = 1;
    char *content = get_tile_content(tile);
    cr_assert_eq(*content, 0);
    my_free(content);
    my_free(tile);
}

Test(get_tile_content, get_no_player_tile_content)
{
    map_tile_t *tile = my_calloc(sizeof(map_tile_t), 1);

    tile->players = NULL;
    tile->eggs = NULL;
    tile->x = 1;
    tile->y = 1;
    tile->resources[LINEMATE] = 1;
    tile->resources[SIBUR] = 4;
    char *content = get_tile_content(tile);
    cr_assert_str_eq(content, "linemate sibur sibur sibur sibur");
    my_free(content);
    my_free(tile);
}

Test(get_tile_content, get_one_player_tile_content)
{
    map_tile_t *tile = my_calloc(sizeof(map_tile_t), 1);
    list_t *player = my_calloc(sizeof(list_t), 1);

    player->next = player;
    tile->players = player;
    tile->eggs = NULL;
    tile->x = 1;
    tile->y = 1;
    tile->resources[LINEMATE] = 1;
    tile->resources[SIBUR] = 4;
    char *content = get_tile_content(tile);
    cr_assert_str_eq(content, "player linemate sibur sibur sibur sibur");
    my_free(content);
    my_free(player);
    my_free(tile);
}
