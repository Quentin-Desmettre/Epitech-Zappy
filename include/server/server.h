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
    #define MAP_SPAWN_FREQ 20
    #define FOOD_CONSUMPTION_FREQ 126
    #define TRY_SYSCALL(var, func, ...) \
if ((var = func(__VA_ARGS__)) == -1) {  \
    *err = perror_str(#func);   \
    return false;               \
}


UNUSED static char *NO_ARGS[2] = {"", NULL};

enum gui_event {
    PLAYER_CONNECTION,
    EXPULSION,
    BROADCAST,
    START_INCANTATION,
    END_INCANTATION,
    EGG_PLANTED,
    RESOURCE_DROP,
    RESOURCE_COLLECT,
    PLAYER_DEATH,
    EGG_READY,
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

typedef struct food_timeout {
    struct timespec end;
    void *cli;
} food_timeout_t;

typedef struct server {
    int fd;
    list_t *clients;
    int client_count;

    // Timeouts
    action_t *actions[MAX_CLIENTS + 1];
    int action_count;
    list_t *food_timeouts;
    struct timespec next_spawn;

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
typedef char *(*gui_cmd_handler_t)(server_t *server, const char *cmd);
typedef struct {
    const char *cmd;
    gui_cmd_handler_t handler;
} gui_cmd_t;

char *gui_map_size_handler(server_t *server, const char *cmd);
char *gui_tile_content_handler(server_t *server, const char *cmd);
char *gui_tiles_content_handler(server_t *server, const char *cmd);
char *gui_team_names_handler(server_t *server, const char *cmd);
char *gui_player_position_handler(server_t *server, const char *cmd);
char *gui_player_level_handler(server_t *server, const char *cmd);
char *gui_player_inventory_handler(server_t *server, const char *cmd);
char *gui_time_request_handler(server_t *server, const char *cmd);
char *gui_time_change_handler(server_t *server, const char *cmd);
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
team_t *get_team_by_name(trantor_t *trantor, const char *team);

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
bool do_action_pre_check(action_t *action, server_t *trantor, client_t *cli);

/**
 * @brief Performs an action.
 *
 * This function executes the action, then removes it from the client wait-list
 * @param action
 * @param server
 * @param cli
 */
void do_action(action_t *action, server_t *trantor);

void handle_actions(server_t *server);
void put_action_in_waitlist(server_t *server, action_t *action);
void notify_gui(server_t *server, enum gui_event event, ...);
void log_ai(client_t *cli, server_t *server, const char *cmd, team_t *team);
void check_food(server_t *server);
void disconnect_client(server_t *server, client_t *cli, bool has_disconnect);
void update_next_spawn(server_t *server);
void send_to_gui(server_t *server, char *msg, bool free_msg);
char *get_gui_message(enum gui_event event, ...);
food_timeout_t *create_food_timeout(int freq, client_t *cli);
client_t *get_client_by_id(server_t *server, int id);

ai_cmd_response_t ai_forward_handler(action_t *action,
    server_t *server, player_t *player);
ai_cmd_response_t ai_right_handler(action_t *action,
    server_t *server, player_t *player);
ai_cmd_response_t ai_left_handler(action_t *action,
    server_t *server, player_t *player);
ai_cmd_response_t ai_look_handler(action_t *action,
    server_t *server, player_t *player);
ai_cmd_response_t ai_inventory_handler(action_t *action,
    server_t *server, player_t *player);
ai_cmd_response_t ai_broadcast_handler(action_t *action,
    server_t *server, player_t *player);
ai_cmd_response_t ai_connect_nbr_handler(action_t *action,
    server_t *server, player_t *player);
ai_cmd_response_t ai_fork_handler(action_t *action,
    server_t *server, player_t *player);
ai_cmd_response_t ai_eject_handler(action_t *action,
    server_t *server, player_t *player);
ai_cmd_response_t ai_take_handler(action_t *action,
    server_t *server, player_t *player);
ai_cmd_response_t ai_set_handler(action_t *action,
    server_t *server, player_t *player);
ai_cmd_response_t ai_incantation_start_handler(action_t *action,
    server_t *server, player_t *player);
ai_cmd_response_t ai_incantation_end_handler(action_t *action,
    server_t *server, player_t *player);
ai_cmd_response_t ai_fork_pre_check(UNUSED action_t *action,
    server_t *server, player_t *player);
int action_cmp(const void *a, const void *b);
struct timespec timespec_add(struct timespec a, struct timeval b);
void send_to_clients_on_tile(server_t *server, char *mess, player_t *player);
void freeze_players(map_tile_t *tile, player_t *player);
void unfreeze_players(server_t *server, map_tile_t *tile, player_t *player);
void check_resource_spawn(server_t *server);

UNUSED static const int requirements_for_level[8][7] = {
        {}, // UNUSED
        {1, 1, 0, 0, 0, 0, 0}, // 1->2
        {2, 1, 1, 1, 0, 0, 0}, // 2->3
        {2, 2, 0, 1, 0, 2, 0}, // 3->4
        {4, 1, 1, 2, 0, 1, 0}, // 4->5
        {4, 1, 2, 1, 3, 0, 0}, // 5->6
        {6, 1, 2, 3, 0, 1, 0}, // 6->7
        {6, 2, 2, 2, 2, 2, 1}, // 7->8
};

#endif //EPITECH_ZAPPY_SERVER_H
