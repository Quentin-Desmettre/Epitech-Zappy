/*
** EPITECH PROJECT, 2023
** Epitech_Zappy
** File description:
** main
*/

#include <stddef.h>
#include <string.h>
#include "server.h"

int main(int ac, char **av)
{
    char *err = NULL;
    server_t *server = init_server(ac, av, &err);

    if (!server) {
        safe_write(2, err, strlen(err));
        safe_write(2, "\n", 1);
        return 84;
    }
    run_server(server);
    destroy_server(server);
    return 0;
}
