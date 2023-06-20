/*
** EPITECH PROJECT, 2023
** Epitech_Zappy
** File description:
** server_init
*/

#include "server.h"
#include "strings.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "utility/strings.h"

static bool init_server_network(server_t *server, char **err)
{
    struct sockaddr_in addr;
    int tmp;

    TRY_SYSCALL(server->fd, socket, AF_INET, SOCK_STREAM, 0);
    addr = (struct sockaddr_in){.sin_family = AF_INET,
        .sin_addr.s_addr = INADDR_ANY, .sin_port = htons(server->params.port)
    };
    TRY_SYSCALL(tmp, setsockopt, server->fd, SOL_SOCKET, SO_REUSEADDR,
                    &(int){1}, sizeof(int));
    TRY_SYSCALL(tmp, bind, server->fd, (struct sockaddr *)&addr, sizeof(addr));
    TRY_SYSCALL(tmp, listen, server->fd, MAX_CLIENTS);
    return true;
}

static void init_read_set(server_t *server)
{
    list_t *tmp = server->clients;

    FD_ZERO(&server->read_fds);
    FD_SET(server->fd, &server->read_fds);
    if (!tmp)
        return;
    do {
        FD_SET(((client_t *)tmp->data)->fd, &server->read_fds);
        tmp = tmp->next;
    } while (tmp != server->clients);
}

void update_next_spawn(server_t *server)
{
    struct timespec now;

    get_time(&now);
    server->next_spawn = get_end_time(MAP_SPAWN_FREQ,
        server->params.freq, now);
}

server_t *init_server(int ac, char **av, char **err)
{
    args_t args = {0};
    server_t *server = NULL;

    if (!get_args(ac, av, &args, err))
        return NULL;
    server = my_calloc(1, sizeof(server_t));
    server->params = args;
    server->trantor = init_trantor(args.width,
        args.height, args.names, args.slots);
    if (!init_server_network(server, err)) {
        destroy_server(server);
        return NULL;
    }
    init_read_set(server);
    update_next_spawn(server);
    spawn_resources(server);
    return server;
}

void destroy_server(server_t *server)
{
    close(server->fd);
    free_list(&server->clients, NULL);
    destroy_trantor(server->trantor);
    free_str_array(server->params.names);
    my_free(server);
}
