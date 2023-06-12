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

map_tile_t *select_tile_for_look_command(trantor_t *trantor, player_t *player, int nb)
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
        return get_tile_by_pos(trantor->map, player->x + nb_on_line, player->y - line);
    if (player->dir == SOUTH)
        return get_tile_by_pos(trantor->map, player->x - nb_on_line, player->y + line);
    if (player->dir == EAST)
        return get_tile_by_pos(trantor->map, player->x + line, player->y + nb_on_line);
    return get_tile_by_pos(trantor->map, player->x - line, player->y - nb_on_line);
}

char *get_list_players(list_t *players)
{
    char *response = NULL;
    char *tmp = NULL;
    list_t *tmp_list = players;
    int count = list_size(players);

    for (int i = 0; i != count; i++) {
        tmp = " player";
        if (response != NULL) {
            response = my_realloc(response, strlen(response) + strlen(tmp) + 1);
        } else {
            response = my_realloc(response, strlen(tmp) + 1);
        }
        sprintf(response, "%s%s", response, tmp);
        tmp_list = tmp_list->next;
    }
    return response;
}

char *get_list_ressources(int ressources[NB_RESOURCE])
{
    char *response = "";

    for (int i = 0; i < NB_RESOURCE; i++) {
        if (ressources[i] != 0)
            response = my_asprintf("%s%s %d ", response, ressources_names[i], ressources[i]);
    }
    return response;
}

int find_direction(player_t *from, player_t *to, trantor_t *trantor)
{
    int vect[2];
    double angle;
    int dir[4] = {0, 90, 180, 270};

    vect[0] = (abs(from->x - to->x) > (trantor->width / 2)) ? ((from->x - to->x) - trantor->width) : (from->x - to->x);
    vect[1] = (abs(from->y - to->y) > (trantor->height / 2)) ? ((from->y - to->y) - trantor->height) : (from->y - to->y);
    angle = atan2(vect[1], vect[0]) * (180 / M_PI);
    angle += 90;
    if (angle < 0)
        angle += 360;
    angle -= dir[to->dir];
    return get_direct_from_angle(angle);
}
