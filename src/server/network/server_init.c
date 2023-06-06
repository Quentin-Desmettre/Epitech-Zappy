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

    if ((server->fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        *err = perror_str("socket");
        return false;
    }
    addr = (struct sockaddr_in){.sin_family = AF_INET,
        .sin_addr.s_addr = INADDR_ANY, .sin_port = htons(server->params.port)
    };
    if (bind(server->fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        *err = perror_str("bind");
        return false;
    }
    if (listen(server->fd, MAX_CLIENTS) < 0) {
        *err = perror_str("listen");
        return false;
    }
    return true;
}

server_t *init_server(int ac, char **av, char **err)
{
    args_t args = {0};
    server_t *server = NULL;

    if (!get_args(ac, av, &args, err))
        return NULL;
    server = my_calloc(1, sizeof(server_t));
    server->params = args;
    server->run = true;
    server->trantor = init_trantor(args.width, args.height);
    if (!init_server_network(server, err)) {
        my_free(server);
        return NULL;
    }
    return server;
}

static void destroy_clients(list_t **clients)
{
    free_list(clients, NULL);
}

void destroy_server(server_t *server)
{
    close(server->fd);
    destroy_clients(&server->clients);
    destroy_trantor(server->trantor);
    free_str_array(server->params.names);
    my_free(server);
}
