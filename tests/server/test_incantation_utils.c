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

Test(count_same_level, count_level_2_and_4)
{
    player_t *player = my_calloc(sizeof(player_t), 1);
    player_t *player2 = my_calloc(sizeof(player_t), 1);
    map_tile_t *tile = my_calloc(sizeof(map_tile_t), 1);
    list_t last = {player, NULL, NULL};
    list_t next = {player2, &last, NULL};
    list_t list = {player, &next, NULL};

    last.next = &list;
    player->level = 2;
    player2->level = 4;
    tile->players = &list;
    cr_assert_eq(count_same_level(player, tile, false), 2);
    cr_assert_eq(count_same_level(player2, tile, false), 1);
    my_free(player);
    my_free(player2);
    my_free(tile);
}

Test(count_same_level, freezed_player)
{
    player_t *player = my_calloc(sizeof(player_t), 1);
    player_t *player2 = my_calloc(sizeof(player_t), 1);
    map_tile_t *tile = my_calloc(sizeof(map_tile_t), 1);
    list_t last = {player, NULL, NULL};
    list_t next = {player2, &last, NULL};
    list_t list = {player, &next, NULL};

    last.next = &list;
    player->level = 2;
    player2->level = 2;
    player2->is_freezed = true;
    tile->players = &list;
    cr_assert_eq(count_same_level(player, tile, false), 2);
    my_free(player);
    my_free(player2);
    my_free(tile);
}

Test(can_level_up, cannot_level_up_to_3)
{
    player_t *player = my_calloc(sizeof(player_t), 1);
    player_t *player2 = my_calloc(sizeof(player_t), 1);
    map_tile_t *tile = my_calloc(sizeof(map_tile_t), 1);
    list_t *players = NULL;

    append_node(&players, player);
    append_node(&players, player2);
    player->level = 2;
    player2->level = 4;
    tile->players = players;
    tile->resources[LINEMATE] = 2;
    tile->resources[DERAUMERE] = 4;
    tile->resources[SIBUR] = 0;
    cr_assert_eq(can_level_up(player, tile, false), false);
    my_free(player);
    my_free(player2);
    my_free(tile);
}

Test(can_level_up, can_level_up_to_3)
{
    player_t *player = my_calloc(sizeof(player_t), 1);
    player_t *player2 = my_calloc(sizeof(player_t), 1);
    map_tile_t *tile = my_calloc(sizeof(map_tile_t), 1);
    list_t last = {player, NULL, NULL};
    list_t next = {player2, &last, NULL};
    list_t list = {player, &next, NULL};

    last.next = &list;
    player->level = 2;
    player2->level = 4;
    tile->players = &list;
    tile->resources[LINEMATE] = 2;
    tile->resources[DERAUMERE] = 4;
    tile->resources[SIBUR] = 2;
    cr_assert_eq(can_level_up(player, tile, false), true);
    my_free(player);
    my_free(player2);
    my_free(tile);
}

Test(do_level_up, level_up)
{
    player_t *player = my_calloc(sizeof(player_t), 1);
    player_t *player2 = my_calloc(sizeof(player_t), 1);
    map_tile_t *tile = my_calloc(sizeof(map_tile_t), 1);
    list_t last = {player, NULL, NULL};
    list_t next = {player2, &last, NULL};
    list_t list = {player, &next, NULL};
    server_t server = {0};

    last.next = &list;
    player->level = 2;
    player2->level = 4;
    tile->players = &list;
    tile->resources[LINEMATE] = 2;
    tile->resources[DERAUMERE] = 4;
    tile->resources[SIBUR] = 2;
    do_level_up(tile, player, &server);
    cr_assert_eq(player->level, 3);
    cr_assert_eq(player2->level, 4);
    cr_assert_eq(tile->resources[LINEMATE], 1);
    cr_assert_eq(tile->resources[DERAUMERE], 3);
    cr_assert_eq(tile->resources[SIBUR], 1);
    my_free(player);
    my_free(player2);
    my_free(tile);
}
