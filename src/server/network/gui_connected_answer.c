/*
** EPITECH PROJECT, 2023
** Epitech-Zappy
** File description:
** gui_connected_answer
*/

#include "server.h"
#include "utility/strings.h"

static char *get_players_connected_message(list_t *clients)
{
    char *answer = my_strdup("");
    client_t *cli;
    size_t len = 0;
    list_t *start = clients;
    player_t *player;

    if (!clients)
        return answer;
    do {
        cli = start->data;
        player = cli->data;
        if (cli->state == AI)
            str_append_free(&answer, &len,
                get_gui_message(PLAYER_CONNECTION,
                player->id, player->x, player->y, player->dir,
                player->level, player->team_name));
        start = start->next;
    } while (start != clients);
    return answer;
}

static char *get_eggs_message(list_t *teams)
{
    char *answer = my_strdup("");
    list_t *team_node = teams;
    team_t *team;
    list_t *egg_node;
    egg_t *egg;
    size_t len = 0;
    for (int i = 0, max = list_size(teams); i < max;
    i++, team_node = team_node->next) {
        team = team_node->data;
        egg_node = team->eggs;
        for (int j = 0, nb_eggs = list_size(team->eggs); j < nb_eggs;
        j++, egg_node = egg_node->next) {
            egg = egg_node->data;
            str_append_free(&answer, &len, egg->is_forked ?
                get_gui_message(EGG_READY,
                    egg->id, egg->player_id, egg->x, egg->y)
                : my_strdup(""));
        }
    }
    return answer;
}

char *get_gui_connected_answer(server_t *server)
{
    size_t len;

    return str_concat_free(&len, 9,
        gui_map_size_handler(server, "msz"), my_strdup("\n"),
        gui_time_request_handler(server, "sgt"), my_strdup("\n"),
        gui_tiles_content_handler(server, "mct"), my_strdup("\n"),
        gui_team_names_handler(server, "tna"),
        get_players_connected_message(server->clients),
        get_eggs_message(server->trantor->teams)
    );
}
