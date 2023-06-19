/*
** EPITECH PROJECT, 2023
** Epitech-Zappy
** File description:
** check_args_validity
*/

#include "server.h"
#include "utility/strings.h"

bool check_args(args_t *args, char **err)
{
    if (args->port <= 0)
        *err = ERR_PORT;
    if (!IS_FREQ_VALID(args->freq))
        *err = ERR_FREQ;
    if (args->width < 10 || args->width > 30)
        *err = ERR_WIDTH;
    if (args->height < 10 || args->height > 30)
        *err = ERR_HEIGHT;
    if (args->slots <= 0)
        *err = ERR_SLOTS;
    if (args->names[0] == NULL)
        *err = ERR_TEAMS;
    if (has_duplicates(args->names))
        *err = ERR_MULTI_TEAMS;
    if (str_array_contains(args->names, GRAPHIC_COMMAND))
        *err = ERR_INVALID_TEAM_NAME;
    return *err == NULL;
}
