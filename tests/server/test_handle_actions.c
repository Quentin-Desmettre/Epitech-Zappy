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

s_list_t **malloc_stack(void);
#define CHECK_ALL_FREE cr_assert_eq(*malloc_stack(), NULL)

Test(put_action_in_waitlist, waitlist_empty)
{
    server_t server = {0};
    action_t ac = {0};

    put_action_in_waitlist(&server, &ac);
    cr_assert(server.action_count == 1);
    cr_assert(server.actions[0] == &ac);
    cr_assert(server.actions[1] == NULL);
}

Test(put_action_in_waitlist, put_5_actions)
{
    server_t server = {0};
    action_t actions[] = {
            {.end_time = {0, 61}},
            {.end_time = {0, 63}},
            {.end_time = {0, 60}},
            {.end_time = {0, 64}},
            {.end_time = {0, 62}},
    };

    for (int i = 0; i < 5; i++)
        put_action_in_waitlist(&server, &actions[i]);
    cr_assert(server.action_count == 5);
    cr_assert_eq(server.actions[0], &actions[2]);
    cr_assert_eq(server.actions[1], &actions[0]);
    cr_assert_eq(server.actions[2], &actions[4]);
    cr_assert_eq(server.actions[3], &actions[1]);
    cr_assert_eq(server.actions[4], &actions[3]);
}

Test(put_action_in_waitlist, multiple_same_time)
{
    server_t server = {0};
    action_t actions[] = {
            {.end_time = {0, 61}},
            {.end_time = {0, 60}},
            {.end_time = {0, 62}},
            {.end_time = {0, 63}},
            {.end_time = {0, 60}},
            {.end_time = {0, 64}},
            {.end_time = {0, 62}},
    };

    for (int i = 0; i < 7; i++)
        put_action_in_waitlist(&server, &actions[i]);
    cr_assert(server.action_count == 7);
    cr_assert_eq(server.actions[0], &actions[1]);
    cr_assert_eq(server.actions[1], &actions[4]);
    cr_assert_eq(server.actions[2], &actions[0]);
    cr_assert_eq(server.actions[3], &actions[2]);
    cr_assert_eq(server.actions[4], &actions[6]);
    cr_assert_eq(server.actions[5], &actions[3]);
    cr_assert_eq(server.actions[6], &actions[5]);
}

Test(handle_actions, no_action_to_do)
{
    server_t server = {0};

    cr_assert(server.action_count == 0);
    handle_actions(&server);
    cr_assert(server.action_count == 0);
}

static ai_cmd_response_t response_ok(void *, trantor_t *, void *)
{
    return AI_CMD_RESPONSE_OK;
}

Test(handle_actions, one_action_not_finished)
{
    struct timespec now; get_time(&now);
    action_t ac = {
            .end_time = {now.tv_sec + 1, now.tv_nsec}
    };
    action_t *ac_to_put = memdup(&ac, sizeof(ac));
    server_t server = {
            .action_count = 1,
            .actions = {ac_to_put, NULL},
    };

    cr_assert(server.action_count == 1);
    handle_actions(&server);
    cr_assert(server.action_count == 1);
    my_free(ac_to_put);
    CHECK_ALL_FREE;
}

static void redirect_all_stdout(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

Test(handle_action, one_action_finished, .init = redirect_all_stdout)
{
    struct timespec now; get_time(&now);
    client_t cli = {.fd = 1};
    cli.data = my_malloc(sizeof(player_t));
    action_t ac = {
            .end_time = {now.tv_sec, now.tv_nsec - 100},
            .cli = &cli,
            .data.handler = (ai_cmd_handler_t)response_ok,
    };
    cli.data->current_action = &ac;
    cli.data->buffered_actions = NULL;
    action_t *ac_to_put = memdup(&ac, sizeof(ac));
    server_t server = {
            .action_count = 1,
            .actions = {ac_to_put, NULL},
    };

    cr_assert(server.action_count == 1);
    handle_actions(&server);
    cr_assert(server.action_count == 0);
    cr_assert_stdout_eq_str("ok\n");
    cr_assert(cli.data->current_action == NULL);
    cr_assert(cli.data->buffered_actions == NULL);
    my_free(cli.data);
    CHECK_ALL_FREE;
}

Test(handle_action, one_action_finished_and_buffer_not_empty)
{
    struct timespec now; get_time(&now);
    client_t cli = {.fd = 1};
    cli.data = my_malloc(sizeof(player_t));
    action_t ac = {
            .end_time = {now.tv_sec, now.tv_nsec - 100},
            .cli = &cli,
            .data.handler = (ai_cmd_handler_t)response_ok,
    };
    action_t *dup_ac1 = memdup(&ac, sizeof(ac));
    action_t *dup_ac2 = memdup(&ac, sizeof(ac));
    cli.data->current_action = &ac;
    cli.data->buffered_actions = NULL;
    cli.data->is_freezed = false;
    append_node(&cli.data->buffered_actions, dup_ac2);
    server_t server = {
            .action_count = 1,
            .actions = {dup_ac1, NULL},
    };

    cr_assert(server.action_count == 1);
    handle_actions(&server);
    cr_assert_eq(server.action_count, 0, "got %d actions left", server.action_count);
    cr_assert_stdout_eq_str("ok\nok\n");
    cr_assert(cli.data->current_action == NULL);
    cr_assert(cli.data->buffered_actions == NULL);
    my_free(cli.data);
    CHECK_ALL_FREE;
}
