/*
** EPITECH PROJECT, 2023
** Epitech_Zappy
** File description:
** test_syscalls
*/

#include <criterion/criterion.h>
#include <unistd.h>
#include <fcntl.h>
#include "utility/strings.h"
#include <criterion/redirect.h>

static void redirect_stdout_err(void)
{
    cr_redirect_stderr();
    cr_redirect_stdout();
}

Test(bytes_available, bytes_available)
{
    int fd = open("/tmp/test_zappy", O_RDWR | O_CREAT | O_TRUNC, 0666);

    cr_assert_neq(fd, -1);
    write(fd, "test", 4);
    close(fd);
    fd = open("/tmp/test_zappy", O_RDONLY);
    cr_assert_neq(fd, -1);
    cr_assert_eq(bytes_available(fd), 4);
}

Test(bytes_available, bytes_available_invalid_fd, .init = redirect_stdout_err)
{
    cr_assert(bytes_available(-1) == -1);
}

Test(get_time, get_time)
{
    struct timespec timeout1;
    get_time(&timeout1);
    usleep(1000000);
    struct timespec timeout2;
    get_time(&timeout2);

    cr_assert(timeout1.tv_sec + 1 == timeout2.tv_sec);
}

Test(select, select)
{
    fd_set read_fds;

    FD_ZERO(&read_fds);
    FD_SET(0, &read_fds);
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;
    cr_assert_eq(try_select(1, &read_fds, NULL, &timeout), 0);
}

Test(select, select_error, .init = redirect_stdout_err)
{
    fd_set read_fds;

    FD_ZERO(&read_fds);
    FD_SET(0, &read_fds);
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;
    close(0);
    cr_assert_eq(try_select(1, &read_fds, NULL, &timeout), -1);
}

Test(is_timespec_equal, exactly_the_same)
{
    struct timespec t1;
    struct timespec t2;

    t1.tv_sec = 1;
    t1.tv_nsec = 0;
    t2.tv_sec = 1;
    t2.tv_nsec = 0;
    cr_assert(is_timespec_equal(&t1, &t2));
}

Test(is_timespec_equal, almost_the_same)
{
    struct timespec t1;
    struct timespec t2;

    t1.tv_sec = 1;
    t1.tv_nsec = 0;
    t2.tv_sec = 1;
    t2.tv_nsec = 100000;
    cr_assert_not(is_timespec_equal(&t1, &t2));
}

Test(is_timespec_equal, 1ms_diff)
{
    struct timespec t1;
    struct timespec t2;

    t1.tv_sec = 1;
    t1.tv_nsec = 0;
    t2.tv_sec = 1;
    t2.tv_nsec = 1000000;
    cr_assert_not(is_timespec_equal(&t1, &t2));
}

Test(is_timespec_equal, sec_diff)
{
    struct timespec t1;
    struct timespec t2;

    t1.tv_sec = 1;
    t1.tv_nsec = 0;
    t2.tv_sec = 2;
    t2.tv_nsec = 0;
    cr_assert_not(is_timespec_equal(&t1, &t2));
}

Test(is_timespec_less, less_seconds)
{
    struct timespec t1;
    struct timespec t2;

    t1.tv_sec = 1;
    t1.tv_nsec = 0;
    t2.tv_sec = 2;
    t2.tv_nsec = 0;
    cr_assert(is_timespec_less(&t1, &t2));
}

Test(is_timespec_less, less_nano)
{
    struct timespec t1;
    struct timespec t2;

    t1.tv_sec = 1;
    t1.tv_nsec = 0;
    t2.tv_sec = 1;
    t2.tv_nsec = 1000000;
    cr_assert(is_timespec_less(&t1, &t2));
}

Test(is_timespec_less, bigger_sec)
{
    struct timespec t1;
    struct timespec t2;

    t1.tv_sec = 2;
    t1.tv_nsec = 0;
    t2.tv_sec = 1;
    t2.tv_nsec = 0;
    cr_assert_not(is_timespec_less(&t1, &t2));
}

Test(is_timespec_less, bigger_nano)
{
    struct timespec t1;
    struct timespec t2;

    t1.tv_sec = 1;
    t1.tv_nsec = 1000000;
    t2.tv_sec = 1;
    t2.tv_nsec = 0;
    cr_assert_not(is_timespec_less(&t1, &t2));
}
