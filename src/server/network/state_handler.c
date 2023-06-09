/*
** EPITECH PROJECT, 2023
** Epitech_Zappy
** File description:
** state_handler
*/

#include "server.h"
#include "utility/strings.h"
#include <string.h>

const gui_cmd_t GUI_HANDLERS[] = {
        {"msz", &gui_map_size_handler},
        {"bct", &gui_tile_content_handler},
        {"mct", &gui_tiles_content_handler},
        {"tna", &gui_team_names_handler},
        {"ppo", &gui_player_position_handler},
        {"plv", &gui_player_level_handler},
        {"pin", &gui_player_inventory_handler},
        {"sgt", &gui_time_request_handler},
        {"sst", &gui_time_change_handler},
};

static char *get_gui_connected_answer(server_t *server)
{
    size_t len;
    char *answer =
            str_concat_free(&len, 4,
                gui_map_size_handler(server, NULL),
                gui_time_request_handler(server, NULL),
                gui_tiles_content_handler(server, NULL),
                gui_team_names_handler(server, NULL)
    );
    list_t *start = server->clients;
    client_t *cli;

    do {
        cli = start->data;
        if (cli->state == AI)
            str_append_free(&answer, &len, gui_pnw_response(cli->data));
    } while (start != server->clients);
    return answer;
}

team_t *get_team_by_name(trantor_t *trantor, const char *team)
{
    list_t *tmp = trantor->teams;
    team_t *tmp_team;

    do {
        tmp_team = tmp->data;
        if (strcmp(tmp_team->name, team) == 0)
            return tmp_team;
        tmp = tmp->next;
    } while (tmp != trantor->teams);
    return NULL;
}

void handle_connected(server_t *server, client_t *cli, const char *cmd)
{
    char *answer = NULL;
    team_t *team;

    if (strcmp(cmd, GRAPHIC_COMMAND) == 0) {
        cli->state = GUI;
        answer = get_gui_connected_answer(server);
        safe_write(cli->fd, answer, strlen(answer));
        return my_free(answer);
    }
    team = get_team_by_name(server->trantor, cmd);
    if (!team || (team->available_slots + team->eggs) == 0) {
        answer = team ? ERR_NO_SLOTS : ERR_NO_TEAM;
        return safe_write(cli->fd, answer, strlen(answer));
    }
    log_ai(cli, server, cmd, team);
}

void handle_gui(server_t *server, UNUSED client_t *cli, const char *cmd)
{
    char **args = str_to_word_array(cmd, " ", NULL);
    char *output;
    size_t len;

    for (int i = 0; GUI_HANDLERS[i].cmd; i++) {
        if (strcmp(GUI_HANDLERS[i].cmd, args[0]) != 0)
            continue;
        output = str_concat_free(&len, 2,
                    GUI_HANDLERS[i].handler(server, args), "\n");
        safe_write(cli->fd, output, len);
        break;
    }
    free_str_array(args);
}
