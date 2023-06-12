/*
** EPITECH PROJECT, 2023
** Epitech_Zappy
** File description:
** trantor
*/

#ifndef EPITECH_ZAPPY_TRANTOR_H
    #define EPITECH_ZAPPY_TRANTOR_H
    #include "utility/linked_list.h"
    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>
    #define ERROR_CODE my_strdup("sbp")
    #define BASE_FOOD 9
    #define UNUSED __attribute__((unused))

//////////////////////////////////////////////////////////////////////////////
// TRANTOR
//////////////////////////////////////////////////////////////////////////////

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

UNUSED static char *ressources_names[NB_RESOURCE] = {
    "food",
    "linemate",
    "deraumere",
    "sibur",
    "mendiane",
    "phiras",
    "thystame"
};

UNUSED static const float RESOURCE_FREQ[NB_RESOURCE] = {
        0.5,
        0.3,
        0.15,
        0.1,
        0.1,
        0.08,
        0.05
};

typedef struct team {
    int available_slots;
    int eggs;
    char *name;
    list_t *players;
    list_t *egg_numbers;
} team_t;

typedef enum direction {
    NORTH,
    EAST,
    SOUTH,
    WEST,
    NB_DIR
} direction_t;

typedef struct map_tile {
    int resources[NB_RESOURCE];
    list_t *players;
    list_t *eggs;
    int x;
    int y;
} map_tile_t;

typedef struct map {
} map_t;

typedef struct trantor {
    list_t *teams;
    dim_list_t *map;
    int width;
    int height;
} trantor_t;

trantor_t *init_trantor(int width, int height,
    char **team_names, int max_players);
void destroy_trantor(trantor_t *trantor);

//////////////////////////////////////////////////////////////////////////////
// AI
//////////////////////////////////////////////////////////////////////////////

typedef enum ai_response_type {
    OK,
    KO,
    TEXT
} ai_response_type_t;

typedef struct ai_cmd_response {
    ai_response_type_t type;
    char *data;
} ai_cmd_response_t;

    #define AI_CMD_RESPONSE_OK (ai_cmd_response_t){OK, NULL}
    #define AI_CMD_RESPONSE_KO (ai_cmd_response_t){KO, NULL}
    #define AI_CMD_RESPONSE_TEXT(text) (ai_cmd_response_t){TEXT, text}

typedef ai_cmd_response_t
        (*ai_cmd_handler_t)(void *action, void *, void *player);
typedef struct action_data {
    int ticks;
    bool has_arg;
    const char *name;
    ai_cmd_handler_t pre_check;
    ai_cmd_handler_t handler;
} action_data_t;
typedef struct action {
    struct timespec start_time;
    struct timespec end_time;
    action_data_t data;
    const char *arg;
    void *cli; // client_t
} action_t;

/**
 * @brief Player structure
 */
typedef struct player {
    int level;
    int x;
    int y;
    int id;
    direction_t dir;

    /**
     * For example, to get the number of food in the inventory,
     * access to inventory[FOOD]
     */
    resource_t inventory[NB_RESOURCE];

    action_t *current_action;
    list_t *buffered_actions;
    team_t *team;
    char *team_name;
    bool is_from_egg;
    bool is_freezed;
    struct timeval time_left;
} player_t;

player_t *create_player(trantor_t *trantor, team_t *team,
                        const char *team_name);
void destroy_player(trantor_t *trantor, player_t *player);

// Ai request handling
extern const action_data_t AI_ACTIONS[];

bool is_action_finished(action_t *action, struct timespec *now);
action_t *create_action(const char *cmd, void *client_t, int f);
void spawn_resources(trantor_t *trantor);

map_tile_t *init_tile(int x, int y);
void link_layers(dim_list_t *map);
map_tile_t *get_tile_by_pos(dim_list_t *map, int x, int y);
team_t *create_team(const char *name, int max_players);
void destroy_team(void *team);
map_tile_t *select_tile_for_look_command(trantor_t *trantor, player_t *player, int nb);
int get_nb_tile(int level);
char *get_tile_content(map_tile_t *tile);

char *get_list_ressources(list_t *ressources);
char *get_list_players(list_t *players);

#endif //EPITECH_ZAPPY_TRANTOR_H
