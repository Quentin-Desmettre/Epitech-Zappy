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
                            client_t *cli, int *i)
{
    action_t *new_action;
    struct timespec now;

    do_action(action, server);
    my_free(action);
    server->action_count--;
    (*i)--;
    cli->data->current_action = NULL;
    memmove(server->actions, server->actions + 1,
        sizeof(action_t *) * server->action_count);
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
        cli = server->actions[0]->cli;
        i++;
        if (!is_action_finished(server->actions[0], &now))
            break;
        if (cli->data->is_freezed)
            continue;
        clear_action(server, server->actions[0], cli, &i);
        if (!cli->data->buffered_actions)
            continue;
        cli->data->current_action = cli->data->buffered_actions->data;
        do_action_pre_check(cli->data->current_action, server, cli);
        put_action_in_waitlist(server, cli->data->current_action);
        remove_node(&cli->data->buffered_actions, 0, NULL);
    }
}
