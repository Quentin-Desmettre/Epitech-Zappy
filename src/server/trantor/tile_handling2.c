/*
** EPITECH PROJECT, 2023
** Epitech_Zappy
** File description:
** tile_handling
*/

#include "trantor.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "utility/garbage_collector.h"
#include "utility/strings.h"

int get_nb_tile(int level)
{
    int nb = 0;

    for (int i = 0; i < level + 1; i++)
        nb += i * 2 + 1;
    return nb;
}

map_tile_t *select_tile_for_look_command(trantor_t *trantor,
                                        player_t *player, int nb)
{
    int line = player->level;
    int nb_on_line;

    while (nb < get_nb_tile(line))
        line--;
    line++;
    nb_on_line = line;
    for (int i = get_nb_tile(line); i != nb + 1; i--)
        nb_on_line--;
    if (player->dir == NORTH)
        return get_tile_by_pos(trantor->map,
                player->x + nb_on_line, player->y - line);
    if (player->dir == SOUTH)
        return get_tile_by_pos(trantor->map,
                player->x - nb_on_line, player->y + line);
    if (player->dir == EAST)
        return get_tile_by_pos(trantor->map,
                player->x + line, player->y + nb_on_line);
    return get_tile_by_pos(trantor->map,
            player->x - line, player->y - nb_on_line);
}

char *  get_list_players(list_t *players)
{
    char *response = my_calloc(1, 1);
    list_t *tmp_list = players;
    int count = list_size(players);

    for (int i = 0; i != count; i++) {
        if (*response)
            str_append(&response, " player");
        else
            str_append(&response, "player");
        tmp_list = tmp_list->next;
    }
    return response;
}

char *get_list_ressources(int ressources[NB_RESOURCE])
{
    char *response = my_calloc(1, 1);
    char *tmp = NULL;

    for (int i = 0; i < NB_RESOURCE; i++) {
        for (int j = 0; j < ressources[i]; j++) {
            tmp = my_asprintf("%s%s%s", response,
                    *response ? " " : "", ressources_names[i]);
            my_free(response);
            response = tmp;
        }
    }
    return response;
}

int find_direction(player_t *from, player_t *to, trantor_t *trantor)
{
    int vect[2];
    double angle;
    int dir[4] = {0, 90, 180, 270};
    int x_diff = from->x - to->x;
    int y_diff = from->y - to->y;

    if (from == to)
        return 0;
    vect[0] = (abs(x_diff) > trantor->width / 2) ?
                (x_diff - trantor->width) : x_diff;
    vect[1] = (abs(y_diff) > trantor->height / 2) ?
                (y_diff - trantor->height) : y_diff;
    angle = atan2(vect[1], vect[0]) * (180 / M_PI);
    angle += 90;
    if (angle < 0)
        angle += 360;
    angle -= dir[to->dir];
    return get_direct_from_angle(angle);
}
