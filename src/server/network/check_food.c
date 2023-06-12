/*
** EPITECH PROJECT, 2023
** Epitech-Zappy
** File description:
** check_food
*/

#include "server.h"
#include "utility/strings.h"

food_timeout_t *create_food_timeout(int freq, client_t *cli)
{
    struct timespec now;
    food_timeout_t *food_timeout = my_calloc(sizeof(food_timeout_t), 1);

    get_time(&now);
    food_timeout->end = get_end_time(FOOD_CONSUMPTION_FREQ, freq, now);
    food_timeout->cli = cli;
    return food_timeout;
}

void check_food(server_t *server)
{
    struct timespec now;
    food_timeout_t *food_time;
    client_t *cli;

    get_time(&now);
    while (true) {
        food_time = server->food_timeouts ?
                    server->food_timeouts->data : NULL;
        if (!food_time || is_timespec_less(&now, &food_time->end))
            break;
        cli = food_time->cli;
        remove_node(&server->food_timeouts, 0, my_free);
        if (cli->data->inventory[FOOD] == 0)
            return disconnect_client(server, cli, false);
        cli->data->inventory[FOOD]--;
        append_node(&server->food_timeouts,
            create_food_timeout(server->params.freq, cli));
    }
}

void check_resource_spawn(server_t *server)
{
    struct timespec now;

    get_time(&now);
    if (is_timespec_less(&now, &server->next_spawn))
        return;
    spawn_resources(server);
    update_next_spawn(server);
}
