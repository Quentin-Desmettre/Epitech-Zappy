/*
** EPITECH PROJECT, 2023
** Epitech-Zappy
** File description:
** server_run_utils
*/

#include "server.h"
#include <sys/socket.h>
#include "utility/strings.h"

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
            nb_level_max += (player->level == LEVEL_MAX ? 1 : 0);
            players = players->next;
        } while (players != ((team_t *)teams->data)->players);
        if (nb_level_max >= NB_MAX_LEVEL_TO_WIN)
            return ((team_t *)teams->data)->name;
    } while (teams != trantor->teams);
    return NULL;
}

double get_action_progress(action_t *action, struct timespec now)
{
    struct timeval action_dur = timespec_diff(action->end_time,
        action->start_time);
    struct timeval time_left = timespec_diff(action->end_time, now);
    size_t action_usec = action_dur.tv_sec * 1000000L + action_dur.tv_usec;
    size_t time_left_usec = time_left.tv_sec * 1000000L + time_left.tv_usec;

    return 1 - (double)time_left_usec / (double)action_usec;
}

void update_server_freq(server_t *server, int new_freq)
{
    double ratio;
    struct timespec now;
    double ticks_left;

    get_time(&now);
    for (int i = 0; i < server->action_count; i++) {
        ratio = get_action_progress(server->actions[i], now);
        ticks_left = server->actions[i]->data.ticks * ratio;
        server->actions[i]->end_time = get_end_time(ticks_left, new_freq, now);
    }
}
