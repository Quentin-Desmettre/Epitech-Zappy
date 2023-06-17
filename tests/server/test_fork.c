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

Test(create_egg, create_two_eggs)
{
    team_t *team = my_calloc(sizeof(team_t), 1);

    cr_assert_eq(create_egg(team), 0);
    cr_assert_eq(team->eggs, 1);
    cr_assert_neq(team->egg_numbers, NULL);
    cr_assert_eq(team->egg_numbers->next, team->egg_numbers);
    cr_assert_eq((int)(long)team->egg_numbers->data, 0);

    cr_assert_eq(create_egg(team), 1);
    cr_assert_eq(team->eggs, 2);
    cr_assert_neq(team->egg_numbers->next, NULL);
    cr_assert_eq(team->egg_numbers->next->next, team->egg_numbers);
    cr_assert_eq((int)(long)team->egg_numbers->data, 0);
    cr_assert_eq((int)(long)team->egg_numbers->next->data, 1);
    my_free(team->egg_numbers->next);
    my_free(team->egg_numbers);
    my_free(team);
}

Test(ai_fork_handler, create_an_egg_and_notify_gui)
{
    team_t *team = my_calloc(sizeof(team_t), 1);
    server_t server = {0};
    player_t player = {0};

    player.team = team;
    cr_assert_eq(ai_fork_handler(NULL, &server, &player).type, OK);
    my_free(team->egg_numbers);
    my_free(team);
}

Test(ai_fork_pre_check, create_an_egg_and_notify_gui)
{
    team_t *team = my_calloc(sizeof(team_t), 1);
    server_t server = {0};
    player_t player = {0};

    player.team = team;
    cr_assert_eq(ai_fork_pre_check(NULL, &server, &player).type, OK);
    my_free(team);
}
