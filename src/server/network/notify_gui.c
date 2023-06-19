/*
** EPITECH PROJECT, 2023
** Epitech_Zappy
** File description:
** notify_gui
*/

#include "server.h"
#include "utility/strings.h"
#include <stdarg.h>
#include <string.h>
#include "utility/strings.h"

const gui_event_t GUI_EVENTS[] = {
        {PLAYER_CONNECTION, "pnw #%d %d %d %d %d %s\n"},
        {EXPULSION, "pex %d\n"},
        {BROADCAST, "pbc %d %s\n"},
        {START_INCANTATION, "pic %d %d %d %s\n"},
        {END_INCANTATION, "pie %d %d %d\n"},
        {EGG_PLANTED, "pfk %d\n"},
        {RESOURCE_DROP, "pdr %d %d\n"},
        {RESOURCE_COLLECT, "pgt %d %d\n"},
        {PLAYER_DEATH, "pdi %d\n"},
        {EGG_READY, "enw %d %d %d %d\n"},
        {EGG_HATCHED, "ebo %d\n"},
        {EGG_DEAD, "edi %d\n"},
        {END_OF_GAME, "seg %s\n"},
        {SERVER_MESSAGE, "smg %s\n"},
        {TILE_CONTENT, "bct %d %d %d %d %d %d %d %d %d\n"},
        {PLAYER_LVL_UP, "plv %d %d\n"},
        {PLAYER_POS, "ppo %d %d %d %d\n"},
        {PLAYER_INVENTORY, "pin %d %d %d %d %d %d %d %d %d %d\n"},
        {TIME_UNIT_CHANGED, "sst %d\n"},
        {NUM_GUI_EVENTS, NULL}
};

void send_to_gui(server_t *server, char *msg, bool free_msg)
{
    list_t *tmp = server->clients;
    client_t *cli;

    debug("Sending to GUIs: %s\n", msg);
    if (!tmp)
        return;
    do {
        cli = (client_t *)tmp->data;
        if (cli->state == GUI)
            safe_write(cli->fd, msg, strlen(msg));
        tmp = tmp->next;
    } while (tmp != server->clients);
    if (free_msg)
        my_free(msg);
}

char *va_get_gui_message(enum gui_event event, va_list args)
{
    const char *format_str = GUI_EVENTS[event].format_str;
    char *msg;
    char *dup;

    if (!format_str)
        return NULL;
    vasprintf(&msg, format_str, args);
    dup = my_strdup(msg);
    free(msg);
    return dup;
}

char *get_gui_message(enum gui_event event, ...)
{
    va_list args;
    char *msg;

    va_start(args, event);
    msg = va_get_gui_message(event, args);
    va_end(args);
    return msg;
}

void notify_gui(server_t *server, enum gui_event event, ...)
{
    va_list args;
    char *msg;

    va_start(args, event);
    msg = va_get_gui_message(event, args);
    va_end(args);
    if (!msg)
        return;
    send_to_gui(server, msg, true);
    my_free(msg);
}
