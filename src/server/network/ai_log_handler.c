/*
** EPITECH PROJECT, 2023
** Epitech-Zappy
** File description:
** ai_log_handler
*/

#include "server.h"
#include "utility/strings.h"
#include <string.h>

static char *get_ai_connected_answer(team_t *team, int map_x, int map_y)
{
    size_t len = 0;
    char slots[10];
    char x_pos[10];
    char y_pos[10];

    sprintf(slots, "%d", team->available_slots);
    sprintf(x_pos, "%d", map_x);
    sprintf(y_pos, "%d", map_y);
    return str_concat(&len, 6, slots, "\n", x_pos, " ", y_pos, "\n");
}

static char *gui_mess_ai_connected(server_t *server, player_t *player)
{
    printf("Player: %p\n", player);
    printf("Team: %p\n", player->team);
    printf("Team name: %s\n", player->team->name);
    char *msg = get_gui_message(PLAYER_CONNECTION,
        player->id, player->x, player->y, player->dir, player->level,
        player->team->name);
    size_t msg_len = strlen(msg);

    str_append_free(&msg, &msg_len, my_strdup("\n"));
    str_append_free(&msg, &msg_len, gui_tiles_content_handler(server, NULL));
    str_append_free(&msg, &msg_len, my_strdup("\n"));
    return msg;
}

void log_ai(client_t *cli, server_t *server, const char *cmd, team_t *team)
{
    player_t *player;
    char *msg = my_strdup("");
    size_t msg_len = 0;
    cli->state = AI;
    cli->data = create_player(server->trantor, team, cmd);
    if (team->eggs > 0) {
        team->eggs--;
        str_append_free(&msg, &msg_len, get_gui_message(EGG_HATCHED,
            (int)(long)team->egg_numbers->data));
        str_append_free(&msg, &msg_len, my_strdup("\n"));
        remove_node(&team->egg_numbers, 0, free);
    } else
        team->available_slots--;
    player = cli->data;
    str_append_free(&msg, &msg_len, gui_mess_ai_connected(server, player));
    safe_write_free(cli->fd, get_ai_connected_answer(team,
        server->params.width, server->params.height));
    append_node(&server->food_timeouts,
        create_food_timeout(server->params.freq, cli));
    send_to_gui(server, msg, true);
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
