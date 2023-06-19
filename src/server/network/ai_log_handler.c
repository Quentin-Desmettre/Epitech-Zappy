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
    return my_asprintf("%d\n%d %d\n", list_size(team->eggs), map_x, map_y);
}

/**
 * @brief Selects an egg from an egg list, prioritizing eggs that are the
 * results of a fork command.
 *
 * @param eggs The egg list. Must be non empty, else the function will crash.
 */
static egg_t *select_egg_for_connection(list_t *eggs)
{
    list_t *start = eggs;
    egg_t *egg;
    egg_t *selected = NULL;

    do {
        egg = start->data;
        if (egg->is_forked)
            return egg;
        if (!selected)
            selected = egg;
        start = start->next;
    } while (start != eggs);
    return selected;
}

void log_ai(client_t *cli, server_t *server, team_t *team)
{
    egg_t *egg = select_egg_for_connection(team->eggs);
    char *gui_mess = NULL;
    size_t len;
    player_t *p;

    cli->state = AI;
    cli->data = create_player(server->trantor, egg);
    p = cli->data;
    gui_mess = egg->is_forked ?
        get_gui_message(EGG_HATCHED, egg->id) : my_strdup("");
    gui_mess = str_concat_free(&len, 2, gui_mess,
        get_gui_message(PLAYER_CONNECTION, p->id, p->x, p->y,
        p->dir, p->level, p->team->name));
    append_node(&server->food_timeouts,
        create_food_timeout(server->params.freq, cli));
    send_to_gui(server, gui_mess, true);
    safe_write_free(cli->fd, get_ai_connected_answer(team,
        server->params.width, server->params.height));
    remove_if(&team->eggs, egg, NULL, my_free);
}

void handle_ai(server_t *server, client_t *cli, const char *cmd)
{
    action_t *action;

    if (list_size(cli->data->buffered_actions) + 1 >= AI_MAX_COMMANDS)
        return;
    action = create_action(cmd, cli, server->params.freq);
    if (!action)
        return safe_write(cli->fd, ERR_NO_CMD, strlen(ERR_NO_CMD));
    if (!cli->data->current_action) {
        do_action_pre_check(action, server, cli);
        cli->data->current_action = action;
        put_action_in_waitlist(server, action);
    } else
        append_node(&cli->data->buffered_actions, action);
}
