/*
** EPITECH PROJECT, 2023
** Epitech_Zappy
** File description:
** main
*/

#include <stddef.h>
#include <string.h>
#include "server.h"

char *perror_str(const char *str)
{
    char *err = my_calloc(1, strlen(str) + strlen(strerror(errno)) + 3);

    strcat(err, str);
    strcat(err, ": ");
    strcat(err, strerror(errno));
    return err;
}

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
