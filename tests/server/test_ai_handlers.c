/*
** EPITECH PROJECT, 2023
** Epitech_Zappy
** File description:
** test_ai_handlers
*/

#define _GNU_SOURCE
#include <stdio.h>
#include <criterion/criterion.h>
#include "trantor.h"
#include "server.h"
#include <time.h>

Test(ai_handlers, inventory)
{
    srand(time(NULL));
    player_t player;
    for (int i = FOOD; i < NB_RESOURCE; i++)
        player.inventory[i] = rand() % 100;
    char *expected;
    asprintf(&expected, "[food %d, linemate %d, deraumere %d, sibur %d, mendiane %d, phiras %d, thystame %d]",
            player.inventory[FOOD],
            player.inventory[LINEMATE],
            player.inventory[DERAUMERE],
            player.inventory[SIBUR],
            player.inventory[MENDIANE],
            player.inventory[PHIRAS],
            player.inventory[THYSTAME]);
    ai_cmd_response_t resp = ai_inventory_handler(NULL, NULL, &player);
    cr_assert(resp.type == TEXT);
    cr_assert(resp.data != NULL);
    cr_assert_str_eq(resp.data, expected);
}

Test(ai_handlers, connect_nbr_only_one_team)
{
    srand(time(NULL));
    int nb_slots = rand() % 100;
    char *nb_slots_str;
    asprintf(&nb_slots_str, "%d", nb_slots);
    team_t *team = create_team("team1", nb_slots, 10, 10);
    player_t player = {.team_name = "team1", .team = team};
    ai_cmd_response_t resp = ai_connect_nbr_handler(NULL, NULL, &player);

    cr_assert(resp.type == TEXT);
    cr_assert(resp.data != NULL);
    cr_assert_str_eq(resp.data, nb_slots_str);
}

Test(ai_handlers, connect_nbr_multiple_teams)
{
    srand(time(NULL));
    int nb_slots[] = {
            rand() % 100,
            rand() % 100,
            rand() % 100
    };
    char *nb_slots_str[3];
    for (int i = 0; i < 3; i++)
        asprintf(&nb_slots_str[i], "%d", nb_slots[i]);

    player_t players[] = {
            {.team_name = "team1"},
            {.team_name = "team2"},
            {.team_name = "team3"}
    };
    for (int i = 0; i < 3; i++) {
        team_t *t = create_team(players[i].team_name, nb_slots[i], 10, 10);
        players[i].team = t;
    }

    for (int i = 0; i < 3; i++) {
        ai_cmd_response_t resp = ai_connect_nbr_handler(NULL, NULL, &players[i]);
        cr_assert(resp.type == TEXT);
        cr_assert(resp.data != NULL);
        cr_assert_str_eq(resp.data, nb_slots_str[i]);
    }
}
