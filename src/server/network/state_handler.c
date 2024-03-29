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
    team_t *team;

    if (strcmp(cmd, GRAPHIC_COMMAND) == 0) {
        debug("New GUI connected\n");
        cli->state = GUI;
        return safe_write_free(cli->fd, get_gui_connected_answer(server));
    }
    team = get_team_by_name(server->trantor, cmd);
    if (!team || !team->eggs) {
        debug("Refused connection for AI\n");
        return safe_write(cli->fd, "ko\n", 3);
    }
    log_ai(cli, server, team);
    debug("New AI connected\n");
}

void handle_gui(server_t *server, UNUSED client_t *cli, const char *cmd)
{
    char *output;
    size_t len;

    for (int i = 0; GUI_HANDLERS[i].cmd; i++) {
        if (strncmp(GUI_HANDLERS[i].cmd, cmd, 3) != 0)
            continue;
        debug("Handling GUI command: %s\n", cmd);
        output = str_concat_free(&len, 2,
                    GUI_HANDLERS[i].handler(server, cmd), my_strdup("\n"));
        return safe_write(cli->fd, output, len);
    }
    debug("Unknown GUI command: %s\n", cmd);
    safe_write(cli->fd, "suc\n", 4);
}
