/*
** EPITECH PROJECT, 2023
** Epitech_Zappy
** File description:
** server_init
*/

#ifndef EPITECH_ZAPPY_SERVER_H
    #define EPITECH_ZAPPY_SERVER_H
    #include <stdlib.h>
    #include <time.h>
    #include <stdbool.h>
    #include <errno.h>
    #include "trantor.h"
    #include "args.h"

    #define MAX_CLIENTS (FD_SETSIZE - 5)

typedef struct server_init {
    int fd;
    list_t *clients;
    trantor_t *trantor;
    args_t params;
} server_t;

// Client handling
typedef enum client_state {
    CONNECTED,
    GUI,
    AI,
} client_state_t;

typedef struct client {
    client_state_t state;
    int fd;
    void *data;
} client_t;

// GUI request handling
typedef char *(*gui_cmd_handler_t)(server_t *server, const char * const *args);
typedef struct {
    const char *cmd;
    gui_cmd_handler_t handler;
} gui_cmd_t;

char *gui_map_size_handler(server_t *server, const char * const * args);
char *gui_tile_content_handler(server_t *server, const char * const * args);
char *gui_tiles_content_handler(server_t *server, const char * const * args);
char *gui_team_names_handler(server_t *server, const char * const * args);
char *gui_player_position_handler(server_t *server, const char * const * args);
char *gui_player_level_handler(server_t *server, const char * const * args);
char *gui_player_inventory_handler(server_t *server, const char * const * args);
char *gui_time_request_handler(server_t *server, const char * const * args);
char *gui_time_change_handler(server_t *server, const char * const * args);

static const gui_cmd_t gui_handlers[] = {
//        {"msz", &gui_map_size_handler},
//        {"bct", &gui_tile_content_handler},
//        {"mct", &gui_tiles_content_handler},
//        {"tna", &gui_team_names_handler},
//        {"ppo", &gui_player_position_handler},
//        {"plv", &gui_player_level_handler},
//        {"pin", &gui_player_inventory_handler},
//        {"sgt", &gui_time_request_handler},
//        {"sst", &gui_time_change_handler},
};

// AI request handling
typedef char *(*ai_cmd_handler_t)(server_t *server, const char * const *args);
typedef struct action_data {
    int ticks;
    const char *name;
    ai_cmd_handler_t handler_start;
    ai_cmd_handler_t handler_end;
} action_data_t;
typedef struct action {
    struct timespec start_time;
    action_data_t data;
} action_t;

char *ai_forward_handler(server_t *server, const char * const * args);
char *ai_right_handler(server_t *server, const char * const * args);
char *ai_left_handler(server_t *server, const char * const * args);
char *ai_look_handler(server_t *server, const char * const * args);
char *ai_inventory_handler(server_t *server, const char * const * args);
char *ai_broadcast_handler(server_t *server, const char * const * args);
char *ai_connect_nbr_handler(server_t *server, const char * const * args);
char *ai_fork_handler(server_t *server, const char * const * args);
char *ai_eject_handler(server_t *server, const char * const * args);
char *ai_take_handler(server_t *server, const char * const * args);
char *ai_set_handler(server_t *server, const char * const * args);
char *ai_incantation_handler(server_t *server, const char * const * args);

/**
 * Logique du serveur
 *
 * - quand un client se connecte:
 *      - creer un client_t, non connecté
 *
 * - quand un client envoie une commande:
 *  - si le client est connecté:
 *    - si la commande est "GRAPHIC":
 *      ouvrir en mode GUI
 *    - si la commande est un des team names:
 *      ouvrir en mode AI
 *    - sinon:
 *      envoyer "ko\n"
 */

server_t *init_server(int ac, char **av, char **err);
void destroy_server(server_t *server);
void run_server(server_t *server);

#endif //EPITECH_ZAPPY_SERVER_H
