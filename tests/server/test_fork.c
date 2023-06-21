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
    team_t *team = create_team("hello", 0, 10, 10);

    egg_t *egg = create_egg(5, 6, false, team);
    cr_assert(egg->is_forked == false);
    cr_assert(egg->x == 5);
    cr_assert(egg->y == 6);
    cr_assert(egg->team == team);
    cr_assert(egg->id == 0);
    cr_assert(egg->player_id == 0);
    cr_assert(list_size(team->eggs) == 1);
    cr_assert(team->eggs->data == egg);

    egg = create_egg(10, 10, true, team);
    cr_assert(egg->is_forked == true);
    cr_assert(egg->x == 10);
    cr_assert(egg->y == 10);
    cr_assert(egg->team == team);
    cr_assert(egg->id == 1);
    cr_assert(egg->player_id == 0);
    cr_assert(list_size(team->eggs) == 2);
    cr_assert(team->eggs->next->data == egg);
}

Test(ai_fork_handler, create_an_egg_and_notify_gui)
{
    team_t *team = my_calloc(sizeof(team_t), 1);
    server_t server = {0};
    player_t player = {0};

    player.team = team;
    cr_assert_eq(ai_fork_handler(NULL, &server, &player).type, OK);
}

Test(ai_fork_pre_check, create_an_egg_and_notify_gui)
{
    team_t *team = my_calloc(sizeof(team_t), 1);
    server_t server = {0};
    player_t player = {0};

    player.team = team;
    ai_cmd_response_t resp = ai_fork_pre_check(NULL, &server, &player);
    cr_assert_eq(resp.type, TEXT);
    cr_assert_str_eq(resp.data, "");
}
