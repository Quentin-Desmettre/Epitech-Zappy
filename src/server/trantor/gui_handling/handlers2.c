/*
** EPITECH PROJECT, 2023
** Epitech_Zappy
** File description:
** handlers2
*/

#include "server.h"
#include "utility/strings.h"
#include "args.h"

client_t *get_client_by_id(server_t *server, int id)
{
    list_t *tmp = server->clients;

    if (!server->clients)
        return NULL;
    do {
        if (((client_t *)tmp->data)->data &&
        ((client_t *)tmp->data)->data->id == id)
            return tmp->data;
        tmp = tmp->next;
    } while (tmp != server->clients);
    return NULL;
}

char *gui_player_level_handler(server_t *server, const char *cmd)
{
    int player_id;
    client_t *cli;

    if (sscanf(cmd, "plv #%d", &player_id) != 1)
        return ERROR_CODE;
    cli = get_client_by_id(server, player_id);
    if (!cli)
        return ERROR_CODE;
    return my_asprintf("plv %d %d", cli->data->id, cli->data->level);
}

char *gui_player_inventory_handler(server_t *server, const char *cmd)
{
    int player_id;
    client_t *cli;
    player_t *player;

    if (sscanf(cmd, "pin #%d", &player_id) != 1)
        return ERROR_CODE;
    cli = get_client_by_id(server, player_id);
    if (!cli)
        return ERROR_CODE;
    player = cli->data;
    return my_asprintf("pin %d %d %d %d %d %d %d %d %d %d", player_id,
        player->x, player->y, player->inventory[0], player->inventory[1],
        player->inventory[2], player->inventory[3], player->inventory[4],
        player->inventory[5], player->inventory[6]);
}

char *gui_time_request_handler(server_t *server, const char *cmd)
{
    if (strcmp(cmd, "sgt") != 0)
        return ERROR_CODE;
    return my_asprintf("sgt %d", server->params.freq);
}

char *gui_time_change_handler(server_t *server, const char *cmd)
{
    int new_freq;

    if (sscanf(cmd, "sst %d", &new_freq) != 1)
        return ERROR_CODE;
    if (!IS_FREQ_VALID(new_freq))
        return ERROR_CODE;
    server->params.freq = new_freq;
    return my_asprintf("sst %d", server->params.freq);
}
