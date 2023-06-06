/*
** EPITECH PROJECT, 2023
** Epitech_Zappy
** File description:
** server_init
*/

#ifndef EPITECH_ZAPPY_SERVER_H
    #define EPITECH_ZAPPY_SERVER_H
    #define _GNU_SOURCE
    #include <stdio.h>
    #include <stdlib.h>
    #include <time.h>
    #include <stdbool.h>
    #include <errno.h>
    #include "trantor.h"
    #include "args.h"
    #include "utility/safe_write.h"
    #include "utility/garbage_collector.h"

    #define UINT64_MAX 18446744073709551615ULL
    #define INT64_MAX 9223372036854775807LL
    #define GRAPHIC_COMMAND "GRAPHIC"
    #define ERR_NO_SLOTS "ko: Not enough opened slots\n"
    #define ERR_NO_TEAM "ko: No such team\n"
    #define ERR_NO_CMD "ko: No such command\n"
    #define AI_MAX_COMMANDS 10
    #define MAX_CLIENTS 1000
    #define WELCOME_MESSAGE "WELCOME\n"
    #define UNUSED __attribute__((unused))

enum gui_event {
    PLAYER_CONNECTION,
    EXPULSION,
    BROADCAST,
    START_INCANTATION,
    END_INCANTATION,
    EGG_LAYED_BY_ME,
    RESOURCE_DROP,
    RESOURCE_COLLECT,
    PLAYER_DEATH,
    EGG_LAYED_BY_OTHER,
    EGG_HATCHED,
    EGG_DEAD,
    END_OF_GAME,
    SERVER_MESSAGE,
    NUM_GUI_EVENTS
};

typedef struct {
    enum gui_event event;
    const char *format_str;
} gui_event_t;

extern const gui_event_t GUI_EVENTS[];

typedef struct server_init {
    int fd;
    list_t *clients;

    // Actions
    action_t *actions[MAX_CLIENTS];
    int action_count;

    trantor_t *trantor;
    args_t params;
    bool run;

    fd_set read_fds;
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
    player_t *data;
    char *buffer;
    size_t buffer_size;
} client_t;

// GUI request handling
typedef char *(*gui_cmd_handler_t)(server_t *server, char **args);
typedef struct {
    const char *cmd;
    gui_cmd_handler_t handler;
} gui_cmd_t;

char *gui_map_size_handler(server_t *server, char **args);
char *gui_tile_content_handler(server_t *server, char **args);
char *gui_tiles_content_handler(server_t *server, char **args);
char *gui_team_names_handler(server_t *server, char **args);
char *gui_player_position_handler(server_t *server, char **args);
char *gui_player_level_handler(server_t *server, char **args);
char *gui_player_inventory_handler(server_t *server, char **args);
char *gui_time_request_handler(server_t *server, char **args);
char *gui_time_change_handler(server_t *server, char **args);
char *gui_pnw_response(player_t *player);

extern const gui_cmd_t GUI_HANDLERS[];

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
void handle_clients(server_t *server, fd_set *read_fds);
team_t *get_team_by_name(server_t *server, const char *team);

// State handling
typedef void (*state_handler_t)(server_t *, client_t *, const char *cmd);
extern const state_handler_t STATE_HANDLER[];
void handle_connected(server_t *server, client_t *cli, const char *cmd);
void handle_gui(server_t *server, UNUSED client_t *cli, const char *cmd);
void handle_ai(server_t *server, client_t *cli, const char *cmd);

/**
 * @brief Performs the pre-check of the action.
 *
 * Once the pre-check has been performed, its output is sent to the client
 * if it failed.
 * @param action
 * @param server
 * @param cli
 * @return True if the action does not have a pre-check or if it
 * was successful, false otherwise.
 */
bool do_action_pre_check(action_t *action, trantor_t *trantor, client_t *cli);

/**
 * @brief Performs an action.
 *
 * This function executes the action, then removes it from the client wait-list
 * @param action
 * @param server
 * @param cli
 */
void do_action(action_t *action, trantor_t *trantor);

void handle_actions(server_t *server);
void put_action_in_waitlist(server_t *server, action_t *action);
void notify_gui(server_t *server, enum gui_event event, ...);
void log_ai(client_t *cli, server_t *server, const char *cmd, team_t *team);

#endif //EPITECH_ZAPPY_SERVER_H
