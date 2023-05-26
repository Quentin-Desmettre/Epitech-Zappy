/*
** EPITECH PROJECT, 2023
** Epitech_Zappy
** File description:
** trantor
*/

#ifndef EPITECH_ZAPPY_TRANTOR_H
    #define EPITECH_ZAPPY_TRANTOR_H
    #include "linked_list.h"

typedef enum resource {
    FOOD,
    LINEMATE,
    DERAUMERE,
    SIBUR,
    MENDIANE,
    PHIRAS,
    THYSTAME,
    NB_RESOURCE
} resource_t;

static const float resource_freq[NB_RESOURCE] = {
        0.5,
        0.3,
        0.15,
        0.1,
        0.1,
        0.08,
        0.05
};

typedef struct map {
} map_t;

typedef struct trantor {
    list_t *teams;
    map_t *map;
} trantor_t;

typedef struct team {
    int available_slots;
    char *name;
    list_t *players;
} team_t;

typedef enum direction {
    NORTH,
    EAST,
    SOUTH,
    WEST
} direction_t;

/**
 * @brief Player structure
 */
typedef struct player {
    int level;
    int x;
    int y;
    direction_t dir;

    /**
     * For example, to get the number of food in the inventory,
     * access to inventory[FOOD]
     */
    resource_t inventory[NB_RESOURCE];

    list_t *actions;
} player_t;

trantor_t *init_trantor(int width, int height);
void destroy_trantor(trantor_t *trantor);

#endif //EPITECH_ZAPPY_TRANTOR_H
