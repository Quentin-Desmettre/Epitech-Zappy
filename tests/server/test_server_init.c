/*
** EPITECH PROJECT, 2023
** Epitech-Zappy
** File description:
** test_server_init
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "server.h"
#include <unistd.h>

s_list_t **malloc_stack(void);
#define CHECK_ALL_FREE cr_assert_eq(*malloc_stack(), NULL)

Test(init_server, simple_init)
{
    srand(time(NULL));
    char rand_port[6];
    sprintf(rand_port, "%d", rand() % 40000 + 20000);
    char *err = NULL;
    server_t *server = init_server(3, (char *[]){"./zappy_server", "-p", rand_port}, &err);

    if (err)
        cr_log_error("%s", err);
    cr_assert(server);
    cr_assert_not(err);
    destroy_server(server);
    CHECK_ALL_FREE;
}

Test(init_server, multiple_server_same_port)
{
    srand(time(NULL));
    char rand_port[6];
    sprintf(rand_port, "%d", rand() % 40000 + 20000);
    char *err = NULL;
    server_t *server = init_server(3, (char *[]){"./zappy_server", "-p", rand_port}, &err);
    cr_assert(server);
    cr_assert_not(err);

    optind = 1;
    server_t *server2 = init_server(3, (char *[]){"./zappy_server", "-p", rand_port}, &err);
    cr_assert_not(server2);
    cr_assert(err);
    destroy_server(server);
    my_free(err);
    CHECK_ALL_FREE;
}

Test(init_server, invalid_args, .init = cr_redirect_stderr)
{
    char *err = NULL;
    server_t *server = init_server(2, (char *[]){"./zappy_server", "-x"}, &err);
    cr_assert_not(server);
    cr_assert_not(err);
    CHECK_ALL_FREE;
}
