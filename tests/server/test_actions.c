/*
** EPITECH PROJECT, 2023
** Epitech-Zappy
** File description:
** test_actions
*/

#include "server.h"
#include <criterion/criterion.h>
#include <criterion/redirect.h>

s_list_t **malloc_stack(void);
#define CHECK_ALL_FREE cr_assert_eq(*malloc_stack(), NULL)

Test(is_action_finished, is_action_finished)
{
    action_t ac = {
            .end_time = {0, 0}
    };
    cr_assert(is_action_finished(&ac, NULL));

    struct timespec now = {1, 1};
    ac.end_time = (struct timespec){2, 2};
    cr_assert(!is_action_finished(&ac, &now));

    ac.end_time = (struct timespec){0, 0};
    now = (struct timespec){0, 1};
    cr_assert(is_action_finished(&ac, &now));

    ac.end_time = (struct timespec){0, 1};
    now = (struct timespec){0, 0};
    cr_assert(!is_action_finished(&ac, &now));
}

Test(create_action, action_non_existing)
{
    cr_assert_eq(create_action("random_shit", NULL, 1), NULL);
}

Test(create_action, action_with_no_argument)
{
    action_t *ac = create_action("Forward", (void *)0xdeadbeef, 1);
    cr_assert(ac);
    cr_assert_eq(ac->data.has_arg, false);
    cr_assert(ac->data.ticks = 7);
    cr_assert_str_eq(ac->data.name, "Forward");
    cr_assert_eq(ac->data.pre_check, NULL);
    cr_assert_eq(ac->data.handler, (ai_cmd_handler_t)ai_forward_handler);
    cr_assert_eq(ac->cli, (void *)0xdeadbeef);
    cr_assert_eq(ac->arg, NULL);
    struct timespec time_diff = {
            ac->end_time.tv_sec - ac->start_time.tv_sec,
            ac->end_time.tv_nsec - ac->start_time.tv_nsec
    };
    cr_assert_eq(time_diff.tv_sec, 7);
    cr_assert_eq(time_diff.tv_nsec, 0);
}

Test(create_action, frequency)
{
    action_t *ac = create_action("Forward", (void *)0xdeadbeef, 10);
    cr_assert(ac);
    cr_assert_eq(ac->data.has_arg, false);
    cr_assert(ac->data.ticks = 7);
    cr_assert_str_eq(ac->data.name, "Forward");
    cr_assert_eq(ac->data.pre_check, NULL);
    cr_assert_eq(ac->data.handler, (ai_cmd_handler_t)ai_forward_handler);
    cr_assert_eq(ac->cli, (void *)0xdeadbeef);
    cr_assert_eq(ac->arg, NULL);
    struct timespec time_diff = {
            ac->end_time.tv_sec - ac->start_time.tv_sec,
            ac->end_time.tv_nsec - ac->start_time.tv_nsec
    };
    while (time_diff.tv_nsec < 0) {
        time_diff.tv_sec--;
        time_diff.tv_nsec += 1000000000;
    }
    cr_assert_eq(time_diff.tv_sec, 0);
    cr_assert_eq(time_diff.tv_nsec, 700000000);
}

Test(create_action, action_with_argument)
{
    action_t *ac = create_action("Take test", (void *)0xdeadbeef, 1);

    cr_assert(ac);
    cr_assert_eq(ac->data.has_arg, true);
    cr_assert(ac->data.ticks = 7);
    cr_assert_str_eq(ac->data.name, "Take");
    cr_assert_eq(ac->data.pre_check, NULL);
    cr_assert_eq(ac->data.handler, (ai_cmd_handler_t)ai_take_handler);
    cr_assert_eq(ac->cli, (void *)0xdeadbeef);
    cr_assert_str_eq(ac->arg, "test");
    struct timespec time_diff = {
            ac->end_time.tv_sec - ac->start_time.tv_sec,
            ac->end_time.tv_nsec - ac->start_time.tv_nsec
    };
    cr_assert_eq(time_diff.tv_sec, 7);
    cr_assert_eq(time_diff.tv_nsec, 0);
}

Test(create_action, arg_supplied_but_not_required)
{
    action_t *ac = create_action("Forward test", (void *)0xdeadbeef, 1);

    cr_assert_eq(ac, NULL);
}

Test(create_action, arg_not_supplied_but_required)
{
    action_t *ac = create_action("Take", (void *)0xdeadbeef, 1);

    cr_assert_eq(ac, NULL);
}

Test(do_action_pre_check, no_check_to_do)
{
    action_t ac = {
            .data.pre_check = NULL
    };
    cr_assert(do_action_pre_check(&ac, NULL, NULL));
}

static ai_cmd_response_t check_failed(void *, trantor_t *, void *)
{
    return AI_CMD_RESPONSE_KO;
}
static ai_cmd_response_t check_succeeded(void *, trantor_t *, void *)
{
    return AI_CMD_RESPONSE_OK;
}
static ai_cmd_response_t check_ok_string(void *, trantor_t *, void *)
{
    return AI_CMD_RESPONSE_TEXT(my_strdup("hello world"));
}

static void redirect_all_stdout(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

Test(do_action_pre_check, pre_check_failed, .init = redirect_all_stdout)
{
    action_t ac = {
            .data.pre_check = &check_failed
    };
    client_t cli = {
            .data = NULL,
            .fd = 1
    };
    cr_assert(!do_action_pre_check(&ac, NULL, &cli));
    cr_assert_stdout_eq_str("ko\n");
}

Test(do_action_pre_check, pre_check_succeeded)
{
    action_t ac = {
            .data.pre_check = &check_succeeded
    };
    client_t cli = {
            .data = NULL
    };
    cr_assert(do_action_pre_check(&ac, NULL, &cli));
}

Test(do_action, check_failed, .init = redirect_all_stdout)
{
    client_t cli = {
            .fd = 1,
            .data = NULL
    };
    action_t ac = {
            .data.handler = &check_failed,
            .cli = &cli
    };

    do_action(&ac, NULL);
    cr_assert_stdout_eq_str("ko\n");
    CHECK_ALL_FREE;
}

Test(do_action, check_succeeded, .init = redirect_all_stdout)
{
    client_t cli = {
            .fd = 1,
            .data = NULL
    };
    action_t ac = {
            .data.handler = &check_succeeded,
            .cli = &cli
    };

    do_action(&ac, NULL);
    cr_assert_stdout_eq_str("ok\n");
    CHECK_ALL_FREE;
}

Test(do_action, check_ok_string, .init = redirect_all_stdout)
{
    client_t cli = {
            .fd = 1,
            .data = NULL
    };
    action_t ac = {
            .data.handler = &check_ok_string,
            .cli = &cli
    };

    do_action(&ac, NULL);
    cr_assert_stdout_eq_str("hello world\n");
    CHECK_ALL_FREE;
}
