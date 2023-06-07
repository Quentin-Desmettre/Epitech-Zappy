/*
** EPITECH PROJECT, 2023
** Epitech-Zappy
** File description:
** ai_log_handler
*/

#include "server.h"
#include "utility/strings.h"
#include <string.h>

static food_timeout_t *create_food_timeout(int freq, client_t *cli)
{
    struct timespec now;
    struct timespec end;
    food_timeout_t *food_timeout = my_calloc(sizeof(food_timeout_t), 1);

    get_time(&now);
    end.tv_sec = now.tv_sec + FOOD_CONSUMPTION_FREQ / freq;
    end.tv_nsec =
            now.tv_nsec + (FOOD_CONSUMPTION_FREQ % freq) * 1000000000L / freq;
    if (end.tv_nsec >= 1000000000L) {
        end.tv_sec++;
        end.tv_nsec -= 1000000000L;
    }
    food_timeout->end = end;
    food_timeout->cli = cli;
    return food_timeout;
}

static char *get_ai_connected_answer(player_t *player, int map_x, int map_y)
{
    size_t len = 0;
    char client_id[10];
    char x_pos[10];
    char y_pos[10];

    sprintf(client_id, "%d", player->id);
    sprintf(x_pos, "%d", map_x);
    sprintf(y_pos, "%d", map_y);
    return str_concat(&len, 6, client_id, "\n", x_pos, " ", y_pos, "\n");
}

void log_ai(client_t *cli, server_t *server, const char *cmd, team_t *team)
{
    char *answer;

    cli->state = AI;
    cli->data = create_player(server->trantor, cmd);
    notify_gui(server, PLAYER_CONNECTION, cli->data->id, cli->data->x,
        cli->data->y, cli->data->dir, cli->data->level, cli->data->team_name);
    team->available_slots--;
    answer = get_ai_connected_answer(cli->data, server->params.width,
        server->params.height);
    safe_write(cli->fd, answer, strlen(answer));
    my_free(answer);
    append_node(&server->food_timeouts,
        create_food_timeout(server->params.freq, cli));
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

void check_food(server_t *server)
{
    struct timespec now;
    food_timeout_t *food_time;
    client_t *cli;

    get_time(&now);
    while (true) {
        food_time = server->food_timeouts ?
            server->food_timeouts->data : NULL;
        if (!food_time || is_timespec_less(&now, &food_time->end))
            break;
        cli = food_time->cli;
        remove_node(&server->food_timeouts, 0, my_free);
        if (cli->data->inventory[FOOD] == 0) {
            safe_write(cli->fd, "dead\n", 5);
            return disconnect_client(server, cli);
        }
        cli->data->inventory[FOOD]--;
        append_node(&server->food_timeouts,
            create_food_timeout(server->params.freq, cli));
    }
}
