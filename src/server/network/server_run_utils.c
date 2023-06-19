/*
** EPITECH PROJECT, 2023
** Epitech-Zappy
** File description:
** server_run_utils
*/

#include "server.h"
#include <sys/socket.h>

void accept_client(server_t *server)
{
    client_t *cli = my_calloc(1, sizeof(client_t));

    cli->fd = accept(server->fd, NULL, NULL);
    if (cli->fd == -1) {
        perror("accept");
        exit(84);
    }
    cli->state = CONNECTED;
    append_node(&server->clients, cli);
    safe_write(cli->fd, WELCOME_MESSAGE, strlen(WELCOME_MESSAGE));
    FD_SET(cli->fd, &server->read_fds);
    server->client_count++;
}

char *get_winning_team(trantor_t *trantor)
{
    list_t *teams = trantor->teams;
    list_t *players;
    player_t *player;
    int nb_level_max;

    do {
        nb_level_max = 0;
        teams = teams->next;
        if (!((team_t *)teams->data)->players)
            continue;
        players = ((team_t *)teams->data)->players;
        do {
            player = players->data;
            nb_level_max += (player->level == 8 ? 1 : 0);
            players = players->next;
        } while (players != ((team_t *)teams->data)->players);
        if (nb_level_max >= 6)
            return ((team_t *)teams->data)->name;
    } while (teams != trantor->teams);
    return NULL;
}
