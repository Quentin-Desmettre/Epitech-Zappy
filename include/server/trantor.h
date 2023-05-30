/*
** EPITECH PROJECT, 2023
** Epitech_Zappy
** File description:
** trantor
*/

#ifndef EPITECH_ZAPPY_TRANTOR_H
    #define EPITECH_ZAPPY_TRANTOR_H
    #include "utility/linked_list.h"

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

static const float resource_freq[NB_RESOURCE] = {
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
    char *name;
    list_t *players;
} team_t;

typedef enum direction {
    NORTH,
    EAST,
    SOUTH,
    WEST
} direction_t;

typedef struct map {
} map_t;

typedef struct trantor {
    list_t *teams;
    map_t *map;
} trantor_t;

trantor_t *init_trantor(int width, int height);
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
} ai_cmd_reponse_t;

    #define AI_CMD_RESPONSE_OK (ai_cmd_reponse_t){OK, NULL}
    #define AI_CMD_RESPONSE_KO (ai_cmd_reponse_t){KO, NULL}
    #define AI_CMD_RESPONSE_TEXT(text) (ai_cmd_reponse_t){TEXT, text}

typedef ai_cmd_reponse_t
        (*ai_cmd_handler_t)(void *action, trantor_t *, void *player);
typedef struct action_data {
    int ticks;
    bool has_arg;
    const char *name;
    ai_cmd_handler_t pre_check;
    ai_cmd_handler_t handler;
} action_data_t;
typedef struct action {
    struct timespec start_time;
    action_data_t data;
    const char *arg;
} action_t;

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

    action_t *current_action;
    list_t *buffered_actions;
} player_t;

player_t *create_player(trantor_t *trantor, const char *team_name);
void destroy_player(trantor_t *trantor, player_t *player);

// Ai request handling
extern const action_data_t AI_ACTIONS[];
ai_cmd_reponse_t ai_forward_handler(action_t *action,
                                    trantor_t *trantor, player_t *player);
ai_cmd_reponse_t ai_right_handler(action_t *action,
                                    trantor_t *trantor, player_t *player);
ai_cmd_reponse_t ai_left_handler(action_t *action,
                                    trantor_t *trantor, player_t *player);
ai_cmd_reponse_t ai_look_handler(action_t *action,
                                    trantor_t *trantor, player_t *player);
ai_cmd_reponse_t ai_inventory_handler(action_t *action,
                                    trantor_t *trantor, player_t *player);
ai_cmd_reponse_t ai_broadcast_handler(action_t *action,
                                    trantor_t *trantor, player_t *player);
ai_cmd_reponse_t ai_connect_nbr_handler(action_t *action,
                                    trantor_t *trantor, player_t *player);
ai_cmd_reponse_t ai_fork_handler(action_t *action,
                                    trantor_t *trantor, player_t *player);
ai_cmd_reponse_t ai_eject_handler(action_t *action,
                                    trantor_t *trantor, player_t *player);
ai_cmd_reponse_t ai_take_handler(action_t *action,
                                    trantor_t *trantor, player_t *player);
ai_cmd_reponse_t ai_set_handler(action_t *action,
                                    trantor_t *trantor, player_t *player);
ai_cmd_reponse_t ai_incantation_start_handler(action_t *action,
                                    trantor_t *trantor, player_t *player);
ai_cmd_reponse_t ai_incantation_end_handler(action_t *action,
                                    trantor_t *trantor, player_t *player);

struct timespec get_action_end(action_t *ac, int f);
void destroy_action(action_t *action);
action_t *create_action(const char *cmd);

#endif //EPITECH_ZAPPY_TRANTOR_H
