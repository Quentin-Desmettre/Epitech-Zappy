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
            .available_slots = 42
    };
    server_t server = {0};

    log_ai(&cli, &server, "team", &team);
    cr_assert(cli.state == AI);
    cr_assert(cli.data);
    cr_assert(team.available_slots == 41);
    cr_assert_stdout_eq_str("0\n0 0\n");
    my_free(cli.data);
    CHECK_ALL_FREE;
}
