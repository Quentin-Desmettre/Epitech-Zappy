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
    printf("Port: %d\n", server->params.port);
    printf("Width: %d\n", server->params.width);
    printf("Height: %d\n", server->params.height);
    printf("Team names: ");
    for (int i = 0; server->params.names[i]; i++)
        printf("%s ", server->params.names[i]);
    printf("\n");
    printf("Slots: %d\n", server->params.slots);
    printf("Freq: %d\n", server->params.freq);
    run_server(server);
    destroy_server(server);
    return 0;
}
