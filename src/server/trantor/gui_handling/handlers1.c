/*
** EPITECH PROJECT, 2023
** Epitech_Zappy
** File description:
** handlers1
*/

#include "server.h"
#include "utility/strings.h"

char *gui_map_size_handler(server_t *server, const char *cmd)
{
    if (strcmp(cmd, "msz") != 0)
        return ERROR_CODE;
    return my_asprintf("msz %d %d",
        server->params.width, server->params.height);
}

static char *get_tile_content_str(server_t *server, int x, int y)
{
    map_tile_t *tile = get_tile_by_pos(server->trantor->map, x, y);

    return my_asprintf("bct %d %d %d %d %d %d %d %d %d", x, y,
        tile->resources[0], tile->resources[1], tile->resources[2],
        tile->resources[3], tile->resources[4], tile->resources[5],
        tile->resources[6]);
}

char *gui_tile_content_handler(server_t *server, const char *cmd)
{
    int x;
    int y;

    if (sscanf(cmd, "bct %d %d", &x, &y) != 2)
        return ERROR_CODE;
    if (x < 0 || x >= server->params.width ||
    y < 0 || y >= server->params.height)
        return ERROR_CODE;
    return get_tile_content_str(server, x, y);
}

char *gui_tiles_content_handler(server_t *server, const char *cmd)
{
    char *answer = NULL;
    size_t len = 0;
    char *tmp;

    if (strcmp(cmd, "mct") != 0)
        return ERROR_CODE;
    for (int y = 0; y < server->params.height; y++) {
        for (int x = 0; x < server->params.width; x++) {
            tmp = get_tile_content_str(server, x, y);
            str_append_free(&answer, &len, tmp);
            str_append(&answer, "\n");
            len++;
        }
    }
    answer[len - 1] = '\0';
    return answer;
}
