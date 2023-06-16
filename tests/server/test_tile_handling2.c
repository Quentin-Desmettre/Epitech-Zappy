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

Test(get_nb_tile, number_of_tiles)
{
    cr_assert_eq(get_nb_tile(0), 1);
    cr_assert_eq(get_nb_tile(1), 4);
    cr_assert_eq(get_nb_tile(2), 9);
    cr_assert_eq(get_nb_tile(3), 16);
    cr_assert_eq(get_nb_tile(4), 25);
    cr_assert_eq(get_nb_tile(5), 36);
    cr_assert_eq(get_nb_tile(6), 49);
    cr_assert_eq(get_nb_tile(7), 64);
    cr_assert_eq(get_nb_tile(8), 81);
}

Test(select_tile_for_look_command, select_tile_north)
{
    trantor_t *trantor = my_calloc(sizeof(trantor_t), 1);
    dim_list_t *list = my_calloc(sizeof(dim_list_t), 1);
    dim_list_t *tile = my_calloc(sizeof(dim_list_t), 1);
    dim_list_t *tile2 = my_calloc(sizeof(dim_list_t), 1);
    player_t *player = my_calloc(sizeof(player_t), 1);

    player->dir = NORTH;
    player->level = 1;
    player->x = 0;
    player->y = 2;
    trantor->map = my_calloc(sizeof(dim_list_t), 1);
    trantor->map->next = list;
    list->data = tile;
    tile->prev = tile2;
    tile2->data = (void *)0xdeadbeef;
    cr_assert_eq(select_tile_for_look_command(trantor, player, 1), 0xdeadbeef);
    my_free(trantor->map);
    my_free(trantor);
    my_free(player);
    my_free(list);
    my_free(tile);
    my_free(tile2);
}

Test(select_tile_for_look_command, select_tile_south)
{
    trantor_t *trantor = my_calloc(sizeof(trantor_t), 1);
    dim_list_t *list = my_calloc(sizeof(dim_list_t), 1);
    dim_list_t *tile = my_calloc(sizeof(dim_list_t), 1);
    dim_list_t *tile2 = my_calloc(sizeof(dim_list_t), 1);
    player_t *player = my_calloc(sizeof(player_t), 1);

    player->dir = SOUTH;
    player->level = 1;
    player->x = -2;
    player->y = 0;
    trantor->map = my_calloc(sizeof(dim_list_t), 1);
    trantor->map->next = list;
    list->data = tile;
    tile->prev = tile2;
    tile2->data = (void *)0xdeadbeef;
    cr_assert_eq(select_tile_for_look_command(trantor, player, 1), 0xdeadbeef);
    my_free(trantor->map);
    my_free(trantor);
    my_free(player);
    my_free(list);
    my_free(tile);
    my_free(tile2);
}

Test(select_tile_for_look_command, select_tile_east)
{
    trantor_t *trantor = my_calloc(sizeof(trantor_t), 1);
    dim_list_t *list = my_calloc(sizeof(dim_list_t), 1);
    dim_list_t *tile = my_calloc(sizeof(dim_list_t), 1);
    dim_list_t *tile2 = my_calloc(sizeof(dim_list_t), 1);
    player_t *player = my_calloc(sizeof(player_t), 1);

    player->dir = EAST;
    player->level = 1;
    player->x = -2;
    player->y = 2;
    trantor->map = my_calloc(sizeof(dim_list_t), 1);
    trantor->map->next = list;
    list->data = tile;
    tile->prev = tile2;
    tile2->data = (void *)0xdeadbeef;
    cr_assert_eq(select_tile_for_look_command(trantor, player, 1), 0xdeadbeef);
    my_free(trantor->map);
    my_free(trantor);
    my_free(player);
    my_free(list);
    my_free(tile);
    my_free(tile2);
}

Test(select_tile_for_look_command, select_tile_west)
{
    trantor_t *trantor = my_calloc(sizeof(trantor_t), 1);
    dim_list_t *list = my_calloc(sizeof(dim_list_t), 1);
    dim_list_t *tile = my_calloc(sizeof(dim_list_t), 1);
    dim_list_t *tile2 = my_calloc(sizeof(dim_list_t), 1);
    player_t *player = my_calloc(sizeof(player_t), 1);

    player->dir = WEST;
    player->level = 1;
    player->x = 0;
    player->y = 0;
    trantor->map = my_calloc(sizeof(dim_list_t), 1);
    trantor->map->next = list;
    list->data = tile;
    tile->prev = tile2;
    tile2->data = (void *)0xdeadbeef;
    cr_assert_eq(select_tile_for_look_command(trantor, player, 1), 0xdeadbeef);
    my_free(trantor->map);
    my_free(trantor);
    my_free(player);
    my_free(list);
    my_free(tile);
    my_free(tile2);
}

Test(get_list_players, select_tile_west)
{
    list_t *players = my_calloc(sizeof(list_t), 1);
    list_t *players2 = my_calloc(sizeof(list_t), 1);

    players->next = players2;
    players2->next = players;
    char *result = get_list_players(players);
    cr_assert_str_eq(result, "player player");
    my_free(result);
    my_free(players);
    my_free(players2);
}

Test(find_direction, same_position)
{
    player_t *player = my_calloc(sizeof(player_t), 1);
    player_t *player2 = my_calloc(sizeof(player_t), 1);
    trantor_t *trantor = my_calloc(sizeof(trantor_t), 1);

    trantor->width = 10;
    trantor->height = 11;
    cr_assert_eq(find_direction(player, player, NULL), 0);
    cr_assert_eq(find_direction(player, player2, trantor), 0);
    my_free(player);
    my_free(player2);
}

Test(find_direction, around_positions)
{
    player_t *player = my_calloc(sizeof(player_t), 1);
    player_t *player2 = my_calloc(sizeof(player_t), 1);
    trantor_t *trantor = my_calloc(sizeof(trantor_t), 1);

    player->x = 5;
    player->y = 2;
    player->dir = EAST;
    player2->x = 2;
    player2->y = 9;
    trantor->width = 10;
    trantor->height = 11;
    cr_assert_eq(find_direction(player2, player, trantor), 4);
    my_free(player);
    my_free(trantor);
}

Test(find_direction, normal_positions)
{
    player_t *player = my_calloc(sizeof(player_t), 1);
    player_t *player2 = my_calloc(sizeof(player_t), 1);
    trantor_t *trantor = my_calloc(sizeof(trantor_t), 1);

    player2->x = 8;
    trantor->width = 10;
    trantor->height = 10;
    cr_assert_eq(find_direction(player2, player, trantor), 3);
    cr_assert_eq(find_direction(player, player2, trantor), 7);
    player2->x = 2;
    player2->y = 2;
    cr_assert_eq(find_direction(player2, player, trantor), 6);
    my_free(player);
    my_free(trantor);
}

