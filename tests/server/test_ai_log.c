/*
** EPITECH PROJECT, 2023
** Epitech-Zappy
** File description:
** test_ai_log
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "server.h"

s_list_t **malloc_stack(void);
#define CHECK_ALL_FREE cr_assert_eq(*malloc_stack(), NULL)

void redirect_all_std(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

Test(log_ai, simple_log, .init = redirect_all_std)
{
    client_t cli = {
            .fd = 1,
    };
    team_t team = {
            .name = "test",
            .eggs = NULL,
            .players = NULL,
    };
    server_t server = {0};
    trantor_t *trantor = init_trantor(10, 10, NO_ARGS, 5);
    server.trantor = trantor;

    create_egg(5, 8, false, &team);
    log_ai(&cli, &server, &team);
    cr_assert(team.players);
    cr_assert(team.eggs == NULL);
    cr_assert(cli.state == AI);
    cr_assert(cli.data);
    cr_assert_stdout_eq_str("0\n0 0\n");
    cr_assert(cli.data->x == 5);
    cr_assert(cli.data->y == 8);
    cr_assert(cli.data->team == &team);
    cr_assert(get_tile_by_pos(trantor->map, 5, 8)->players->data = cli.data);
}
