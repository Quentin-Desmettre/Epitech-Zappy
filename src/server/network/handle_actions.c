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

static void clear_action(server_t *server, action_t *action, client_t *cli)
{
    do_action(action, server->trantor);
    my_free(action);
    server->action_count--;
    cli->data->current_action = NULL;
    memmove(server->actions, server->actions + 1,
        sizeof(action_t *) * server->action_count);
}

void handle_actions(server_t *server)
{
    action_t *action;
    struct timespec now;
    client_t *cli;
    get_time(&now);
    while (server->action_count) {
        action = server->actions[0];
        cli = action->cli;
        if (!is_action_finished(action, &now))
            break;
        clear_action(server, action, cli);
        if (!cli->data->buffered_actions)
            continue;
        cli->data->current_action = cli->data->buffered_actions->data;
        put_action_in_waitlist(server, cli->data->current_action);
        remove_node(&cli->data->buffered_actions, 0, NULL);
    }
}
