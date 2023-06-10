/*
** EPITECH PROJECT, 2023
** Epitech_Zappy
** File description:
** player
*/

#include "trantor.h"
#include "utility/garbage_collector.h"

static void place_player(trantor_t *trantor, player_t *player)
{
    int x = random() % trantor->width;
    int y = random() % trantor->height;
    map_tile_t *tile;

    player->x = x;
    player->y = y;
    tile = get_tile_by_pos(trantor->map, x, y);
    append_node(&tile->players, player);
}

player_t *create_player(trantor_t *trantor, team_t *team,
                        const char *team_name)
{
    static int player_id = 0;
    player_t *p = my_calloc(sizeof(player_t), 1);

    p->level = 1;
    p->id = player_id++;
    p->dir = random() % NB_DIR;
    p->inventory[FOOD] = BASE_FOOD;
    p->team = team;
    p->team_name = my_strdup(team_name);
    p->is_from_egg = team->egg_numbers ? true : false;
    append_node(&team->players, p);
    place_player(trantor, p);
    return p;
}

void destroy_player(trantor_t *trantor, player_t *player)
{
    my_free(player->team_name);
    remove_if(&player->team->players, player, NULL, NULL);
    remove_if(&get_tile_by_pos(trantor->map, player->x, player->y)->players,
        player, NULL, NULL);
    my_free(player);
}
