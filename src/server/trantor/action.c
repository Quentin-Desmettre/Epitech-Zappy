/*
** EPITECH PROJECT, 2023
** Epitech_Zappy
** File description:
** action
*/

#include "server.h"
#include <string.h>
#include "utility/strings.h"

const action_data_t AI_ACTIONS[] = {
        {7, false, "Forward", NULL, (ai_cmd_handler_t)ai_forward_handler},
        {7, false, "Right", NULL, (ai_cmd_handler_t)ai_right_handler},
        {7, false, "Left", NULL, (ai_cmd_handler_t)ai_left_handler},
        {7, false, "Look", NULL, (ai_cmd_handler_t) ai_look_handler},
        {1, false, "Inventory", NULL, (ai_cmd_handler_t)ai_inventory_handler},
        {0, true, "Broadcast", NULL, (ai_cmd_handler_t)ai_broadcast_handler},
        {7, false, "Connect_nbr", NULL,
                                    (ai_cmd_handler_t)ai_connect_nbr_handler},
        {42, false, "Fork",
                        (ai_cmd_handler_t)ai_fork_pre_check,
                        (ai_cmd_handler_t)ai_fork_handler},
        {7, false, "Eject", NULL, (ai_cmd_handler_t)ai_eject_handler},
        {7, true, "Take", NULL, (ai_cmd_handler_t)ai_take_handler},
        {7, true, "Set", NULL, (ai_cmd_handler_t)ai_set_handler},
        {300, false, "Incantation",
                        (ai_cmd_handler_t)ai_incantation_start_handler,
                        (ai_cmd_handler_t)ai_incantation_end_handler},
        {0, 0, NULL, NULL, NULL}
};

static action_t *init_action(const action_data_t *data,
                    const char *arg, client_t *client, int f)
{
    action_t *action = my_calloc(sizeof(action_t), 1);
    struct timespec end_time;

    action->data = *data;
    action->arg = arg ? my_strdup(arg + 1) : NULL;
    action->cli = client;
    get_time(&action->start_time);
    end_time = (struct timespec){
            action->start_time.tv_sec + data->ticks / f,
            action->start_time.tv_nsec + (data->ticks % f) * 1000000000L / f
    };
    if (end_time.tv_nsec >= 1000000000) {
        end_time.tv_sec++;
        end_time.tv_nsec -= 1000000000;
    }
    action->end_time = end_time;
    return action;
}

bool is_action_finished(action_t *action, struct timespec *now)
{
    struct timespec my_now;

    if (!now) {
        get_time(&my_now);
        now = &my_now;
    }
    return now->tv_sec > action->end_time.tv_sec ||
    (now->tv_sec == action->end_time.tv_sec &&
    now->tv_nsec >= action->end_time.tv_nsec);
}

action_t *create_action(const char *cmd, void *client, int f)
{
    char *argument = strchr(cmd, ' ');
    char *cmd_name = argument ? strndup(cmd, argument - cmd) : strdup(cmd);

    if (!cmd_name)
        return NULL;
    for (int i = 0; AI_ACTIONS[i].name; i++) {
        if (strcmp(cmd_name, AI_ACTIONS[i].name) != 0)
            continue;
        free(cmd_name);
        if ((argument && !AI_ACTIONS[i].has_arg) ||
        (!argument && AI_ACTIONS[i].has_arg))
            return NULL;
        return init_action(&AI_ACTIONS[i], argument, client, f);
    }
    free(cmd_name);
    return NULL;
}

void do_action(action_t *action, server_t *server)
{
    ai_cmd_response_t resp;
    char *answer;
    client_t *cli = action->cli;
    size_t len = 3;

    debug("Doing action %s for player %d\n", action->data.name, cli->data->id);
    resp = action->data.handler(action, server, cli->data);
    if (resp.type == KO)
        answer = my_strdup("ko\n");
    else if (resp.type == OK)
        answer = my_strdup("ok\n");
    else
        answer = str_concat(&len, 2, resp.data, "\n");
    safe_write(cli->fd, answer, len);
    my_free(resp.data);
    my_free(answer);
}

bool do_action_pre_check(action_t *action, server_t *server, client_t *cli)
{
    ai_cmd_response_t resp;
    char *answer;
    size_t len = 3;

    if (!action->data.pre_check)
        return true;
    resp = action->data.pre_check(action, server, cli->data);
    if (resp.type == KO)
        answer = my_strdup("ko\n");
    else if (resp.type == OK)
        answer = my_strdup("ok\n");
    else
        answer = str_concat(&len, 2, resp.data, "\n");
    if (strlen(answer) - 1)
        safe_write(cli->fd, answer, len);
    my_free(resp.data);
    my_free(answer);
    return resp.type != KO;
}
