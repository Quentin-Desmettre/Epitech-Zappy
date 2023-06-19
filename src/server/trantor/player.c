/*
** EPITECH PROJECT, 2023
** Epitech_Zappy
** File description:
** player
*/

#include "trantor.h"
#include "utility/garbage_collector.h"

player_t *create_player(trantor_t *trantor, egg_t *egg)
{
    static int player_id = 0;
    player_t *p = my_calloc(sizeof(player_t), 1);
    team_t *team = egg->team;
    map_tile_t *tile = get_tile_by_pos(trantor->map, egg->x, egg->y);

    p->id = player_id++;
    p->level = 1;
    p->x = egg->x;
    p->y = egg->y;
    p->dir = random() % NB_DIR;
    p->inventory[FOOD] = BASE_FOOD;
    p->team = egg->team;
    p->team_name = my_strdup(team->name);
    p->is_from_egg = egg->is_forked;
    append_node(&team->players, p);
    append_node(&tile->players, p);
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
