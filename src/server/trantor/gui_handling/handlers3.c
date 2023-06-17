/*
** EPITECH PROJECT, 2023
** Epitech-Zappy
** File description:
** handlers3
*/

#include "server.h"
#include "utility/strings.h"

char *gui_team_names_handler(server_t *server, const char *cmd)
{
    char *answer;
    size_t len = 0;

    if (strcmp(cmd, "tna") != 0)
        return ERROR_CODE;
    answer = my_strdup("");
    for (int i = 0; server->params.names[i]; i++) {
        str_append_free(&answer, &len,
            my_asprintf("tna %s\n", server->params.names[i]));
    }
    return answer;
}

char *gui_player_position_handler(server_t *server, const char *cmd)
{
    client_t *cli = NULL;
    int id;

    if (sscanf(cmd, "ppo #%d", &id) != 1 || !server->clients)
        return ERROR_CODE;
    cli = get_client_by_id(server, id);
    if (!cli)
        return ERROR_CODE;
    return my_asprintf("ppo %d %d %d %d", cli->data->id, cli->data->x,
        cli->data->y, cli->data->dir);
}
