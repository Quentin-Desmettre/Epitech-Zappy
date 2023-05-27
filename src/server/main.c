/*
** EPITECH PROJECT, 2023
** Epitech_Zappy
** File description:
** main
*/

#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "server.h"

char *perror_str(const char *str)
{
    char *err = calloc(1, strlen(str) + strlen(strerror(errno)) + 3);

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
        if (err)
            fprintf(stderr, "%s\n", err);
        return 84;
    }
    run_server(server);
    destroy_server(server);
    return 0;
}
