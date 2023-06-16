/*
** EPITECH PROJECT, 2023
** Epitech_Zappy
** File description:
** test_safe_write
*/

#include <criterion/criterion.h>
#include "utility/safe_write.h"
#include "utility/garbage_collector.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <time.h>
#include <criterion/redirect.h>
#include "utility/strings.h"
#include <pthread.h>
#include <errno.h>

s_list_t **malloc_stack(void);
#define CHECK_ALL_FREE cr_assert_eq(*malloc_stack(), NULL)

#define assert_file_content(path, eq) \
    int __fd__ = open(path, O_RDONLY);     \
    struct stat st;                        \
    stat(path, &st);                       \
    char *content = my_calloc(1, st.st_size + 1); \
    read(__fd__, content, st.st_size);     \
    cr_assert_str_eq(content, eq);         \
    my_free(content)

static void redirect_stdout_err(void)
{
    cr_redirect_stderr();
    cr_redirect_stdout();
}

Test(test_safe_write, test_normal, .init = redirect_stdout_err)
{
    int fd = open("/tmp/zappy_test", O_RDWR | O_TRUNC | O_CREAT, 0666);

    cr_assert_neq(fd, -1);
    safe_write(fd, "test", 4);
    close(fd);
    assert_file_content("/tmp/zappy_test", "test");
}

int create_server_fd(int *port)
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    cr_assert_neq(fd, -1);
    struct sockaddr_in addr;
    do {
        *port = rand() % (60000 - 2000) + 2000;
        addr = (struct sockaddr_in) {.sin_family = AF_INET,
                .sin_addr.s_addr = INADDR_ANY, .sin_port = htons(*port)
        };
    } while (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0);
    if (listen(fd, 10) < 0)
        cr_assert_fail();
    return fd;
}

void *test_accept_client(void *fd)
{
    int r_fd = (long)fd;
    char buf[1024];

    struct sockaddr_in addr;
    socklen_t size;
    int tmp = accept(r_fd, (struct sockaddr *)&addr, &size);
    while (read(tmp, buf, sizeof(buf)) > 0);
    return NULL;
}

#include <string.h>
int connect_to_server(int port)
{
    int cli_fd = socket(AF_INET, SOCK_STREAM, 0);

    cr_assert_neq(cli_fd, -1);
    struct sockaddr_in addr = (struct sockaddr_in){.sin_family = AF_INET,
            .sin_addr.s_addr = INADDR_ANY, .sin_port = htons(port)
    };
    if (connect(cli_fd, (const struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1) {
        perror("connect");
        cr_assert_fail();
    }
    return cli_fd;
}

Test(test_safe_write, test_enormous_write)
{
    srand(time(NULL));
    int port;
    int fd = create_server_fd(&port);
    pthread_t thread;
    pthread_create(&thread, NULL, test_accept_client, (void *)(long)fd);
    int cli_fd = connect_to_server(port);

    char *bigstr = malloc(100000000);
    memset(bigstr, 'a', 100000000);
    safe_write(cli_fd, bigstr, 100000000);
    free(bigstr);
    cr_assert((*packet_waitlist()) != NULL);

    while (*packet_waitlist()) {
        fd_set write_fds;
        FD_ZERO(&write_fds);
        FD_SET(cli_fd, &write_fds);
        write_packets_for_fds(&write_fds);
    }
    cr_assert((*packet_waitlist()) == NULL);
    CHECK_ALL_FREE;
    close(cli_fd);
    close(fd);
    pthread_join(thread, NULL);
}

Test(write_packets_for_fds, empty_waitlist)
{
    cr_assert((*packet_waitlist()) == NULL);
    fd_set write_fds;
    FD_ZERO(&write_fds);
    FD_SET(2, &write_fds);
    write_packets_for_fds(&write_fds);
    cr_assert((*packet_waitlist()) == NULL);
}

static void set_socket_blocking(int fd, bool block)
{
    int flags = fcntl(fd, F_GETFL, 0);

    if (flags == -1) {
        perror("fcntl");
        exit(84);
    }
    if (block)
        flags &= ~O_NONBLOCK;
    else
        flags |= O_NONBLOCK;
    if (fcntl(fd, F_SETFL, flags) == -1) {
        perror("fcntl");
        exit(84);
    }
}

void fill_fd(int fd)
{
    char buf[30000];

    set_socket_blocking(fd, false);
    memset(buf, 'a', 30000);
    while (errno != EAGAIN && errno != EWOULDBLOCK)
        write(fd, buf, 30000);
    set_socket_blocking(fd, true);
}

void *accept_client_no_read(void *fd)
{
    int r_fd = (long)fd;

    struct sockaddr_in addr;
    socklen_t size;
    int tmp = accept(r_fd, (struct sockaddr *)&addr, &size);
    return NULL;
}

Test(write_packets_for_fds, dest_fd_is_busy)
{
    srand(time(NULL));
    int port;
    int fd = create_server_fd(&port);
    pthread_t thread;
    pthread_create(&thread, NULL, accept_client_no_read, (void *)(long)fd);
    int cli_fd = connect_to_server(port);

    fill_fd(cli_fd);
    safe_write(cli_fd, "test", 4);
    cr_assert((*packet_waitlist()) != NULL);
    close(cli_fd);
    close(fd);
    pthread_join(thread, NULL);
}