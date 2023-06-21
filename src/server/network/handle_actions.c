/*
** EPITECH PROJECT, 2023
** Epitech_Zappy
** File description:
** handle_actions
*/

#include <string.h>
#include "server.h"
#include "utility/strings.h"

static void insert_action(server_t *server, int index, action_t *action)
{
    for (int i = server->action_count; i > index; i--)
        server->actions[i] = server->actions[i - 1];
    server->actions[index] = action;
    server->action_count++;
}

void put_action_in_waitlist(server_t *server, action_t *action)
{
    int start = 0;
    int end = server->action_count;
    int mid;
    action_t *cmp_ac;

    while (start < end) {
        mid = start + (end - start) / 2;
        cmp_ac = server->actions[mid];
        if (is_timespec_equal(&action->end_time, &cmp_ac->end_time))
            return insert_action(server, mid + 1, action);
        if (is_timespec_less(&action->end_time, &cmp_ac->end_time))
            end = mid;
        else
            start = mid + 1;
    }
    insert_action(server, start, action);
}

static void clear_action(server_t *server, action_t *action,
                            client_t *cli)
{
    action_t *new_action;
    struct timespec now;
    int index;

    do_action(action, server);
    for (index = 0; index < server->action_count; index++)
        if (server->actions[index] == action)
            break;
    memmove(server->actions + index, server->actions + index + 1,
        sizeof(action_t *) * (server->action_count - index - 1));
    my_free(action);
    cli->data->current_action = NULL;
    server->action_count--;
    sort_actions(server);
    if (!cli->data->buffered_actions)
        return;
    new_action = cli->data->buffered_actions->data;
    get_time(&now);
    new_action->end_time = get_end_time(new_action->data.ticks,
        server->params.freq, now);
}

void handle_actions(server_t *server)
{
    struct timespec now;
    client_t *cli;
    int i = 0;

    get_time(&now);
    while (server->action_count && i < server->action_count) {
        i++;
        cli = server->actions[i - 1]->cli;
        if (!is_action_finished(server->actions[i - 1], &now))
            break;
        if (cli->data->is_freezed)
            continue;
        clear_action(server, server->actions[i - 1], cli);
        i = 0;
        if (!cli->data->buffered_actions)
            continue;
        pop_waitlist(server, cli);
    }
}
