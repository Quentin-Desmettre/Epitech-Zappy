/*
** EPITECH PROJECT, 2023
** Epitech-Zappy
** File description:
** ai_log_handler
*/

#include "server.h"
#include "utility/strings.h"
#include <string.h>

static char *get_ai_connected_answer(player_t *player)
{
    size_t len = 0;
    char client_id[10];
    char x_pos[10];
    char y_pos[10];

    sprintf(client_id, "%d", player->id);
    sprintf(x_pos, "%d", player->x);
    sprintf(y_pos, "%d", player->y);
    return str_concat(&len, 6, client_id, "\n", x_pos, " ", y_pos, "\n");
}

void log_ai(client_t *cli, server_t *server, const char *cmd, team_t *team)
{
    char *answer = get_ai_connected_answer(cli->data);

    cli->state = AI;
    cli->data = create_player(server->trantor, cmd);
    notify_gui(server, PLAYER_CONNECTION, cli->data->id, cli->data->x,
               cli->data->y, cli->data->dir, cli->data->level, cli->data->team_name);
    team->available_slots--;
    safe_write(cli->fd, answer, strlen(answer));
    my_free(answer);
}

void handle_ai(server_t *server, client_t *cli, const char *cmd)
{
    action_t *action;

    if (list_size(cli->data->buffered_actions) + 1 >= AI_MAX_COMMANDS)
        return;
    action = create_action(cmd, cli, server->params.freq);
    if (!action)
        return safe_write(cli->fd, ERR_NO_CMD, strlen(ERR_NO_CMD));
    if (!do_action_pre_check(action, server->trantor, cli))
        return free(action);
    if (action->data.ticks == 0) {
        do_action(action, server->trantor);
        return free(action);
    }
    if (!cli->data->current_action) {
        cli->data->current_action = action;
        put_action_in_waitlist(server, action);
    } else
        append_node(&cli->data->buffered_actions, action);
}
