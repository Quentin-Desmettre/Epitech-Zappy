/*
** EPITECH PROJECT, 2023
** Epitech-Zappy
** File description:
** map
*/

#include "server.h"

static void add_int_array(int array_dest[NB_RESOURCE],
                    const int array_to_add[NB_RESOURCE])
{
    for (int i = 0; i < NB_RESOURCE; i++)
        array_dest[i] += array_to_add[i];
}

static void fetch_resource_to_spawn(trantor_t *trantor,
                                    int to_spawn[NB_RESOURCE])
{
    dim_list_t *row = trantor->map;
    dim_list_t *col;
    map_tile_t *tile;
    int resources[NB_RESOURCE] = {0};
    int map_dim = trantor->width * trantor->height;

    for (int i = 0; i < trantor->height; i++, row = row->next) {
        col = row->data;
        for (int j = 0; j < trantor->width; j++, col = col->next) {
            tile = col->data;
            add_int_array(resources, tile->resources);
        }
    }
    for (int i = 0; i < NB_RESOURCE; i++)
        to_spawn[i] = (int)(RESOURCE_FREQ[i] * (float)map_dim) - resources[i];
}

static void spawn_resource(trantor_t *trantor, resource_t resource,
    map_tile_t *has_spawned[trantor->width][trantor->height])
{
    int rand_x = random() % trantor->width;
    int rand_y = random() % trantor->height;
    map_tile_t *tile = get_tile_by_pos(trantor->map, rand_x, rand_y);

    tile->resources[resource] += 1;
    has_spawned[rand_x][rand_y] = tile;
}

static void notify_resource_spawn(server_t *server, int width, int height,
    map_tile_t *has_spawned[width][height])
{
    map_tile_t *tile;

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            tile = has_spawned[i][j];
            tile ? notify_gui(server, TILE_CONTENT, tile->x, tile->y,
                tile->resources[FOOD], tile->resources[LINEMATE],
                tile->resources[DERAUMERE], tile->resources[SIBUR],
                tile->resources[MENDIANE], tile->resources[PHIRAS],
                tile->resources[THYSTAME])
            : 0;
        }
    }
}

void spawn_resources(server_t *server)
{
    int width = server->trantor->width;
    int height = server->trantor->height;
    int ressources_to_spawn[NB_RESOURCE] = {0};
    map_tile_t *has_spawned[width][height];

    memset(has_spawned, 0, sizeof(has_spawned));
    fetch_resource_to_spawn(server->trantor, ressources_to_spawn);
    for (int i = 0; i < NB_RESOURCE; i++) {
        for (int j = 0; j < ressources_to_spawn[i]; j++)
            spawn_resource(server->trantor, i, has_spawned);
    }
    notify_resource_spawn(server, width, height, has_spawned);
}
