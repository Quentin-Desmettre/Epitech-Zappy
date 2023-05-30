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
        {42, false, "Fork", NULL, (ai_cmd_handler_t)ai_fork_handler},
        {7, false, "Eject", NULL, (ai_cmd_handler_t)ai_eject_handler},
        {7, true, "Take", NULL, (ai_cmd_handler_t)ai_take_handler},
        {7, true, "Set", NULL, (ai_cmd_handler_t)ai_set_handler},
        {300, false, "Incantation",
                        (ai_cmd_handler_t)ai_incantation_start_handler,
                        (ai_cmd_handler_t)ai_incantation_end_handler},
        {0, 0, NULL, NULL, NULL}
};

struct timespec get_action_end(action_t *ac, int f)
{
    return (struct timespec){
            ac->start_time.tv_sec + ac->data.ticks / f,
            ac->start_time.tv_nsec + (ac->data.ticks % f) * 1000000000 / f
    };
}

void destroy_action(action_t *action)
{
    my_free(action);
}

action_t *create_action(const char *cmd)
{
    char *argument = strchr(cmd, ' ');
    char *cmd_name = argument ? strndup(cmd, argument - cmd) : strdup(cmd);
    action_t *action;

    for (int i = 0; AI_ACTIONS[i].name; i++) {
        if (strcmp(cmd_name, AI_ACTIONS[i].name) != 0)
            continue;
        free(cmd_name);
        if ((argument && !AI_ACTIONS[i].has_arg) ||
        (!argument && AI_ACTIONS[i].has_arg) || strlen(argument))
            return NULL;
        action = my_calloc(sizeof(action_t), 1);
        action->data = AI_ACTIONS[i];
        action->arg = argument ? my_strdup(argument + 1) : NULL;
        return action;
    }
    free(cmd_name);
    return NULL;
}

void do_action(action_t *action, trantor_t *trantor, client_t *cli)
{
    ai_cmd_reponse_t resp;
    char *answer;
    size_t len = 3;

    resp = action->data.handler(action, trantor, cli->data);
    if (resp.type == KO)
        answer = my_strdup("ko\n");
    else if (resp.type == OK)
        answer = my_strdup("ok\n");
    else
        answer = str_concat(&len, 2, resp.data, "\n");
    safe_write(cli->fd, answer, len);
    free(resp.data);
    free(answer);
}

bool do_action_pre_check(action_t *action, trantor_t *trantor, client_t *cli)
{
    ai_cmd_reponse_t resp;

    if (!action->data.pre_check)
        return true;
    resp = action->data.pre_check(action, trantor, cli->data);
    free(resp.data);
    if (resp.type == KO) {
        safe_write(cli->fd, "ko\n", 3);
        return false;
    }
    return true;
}
