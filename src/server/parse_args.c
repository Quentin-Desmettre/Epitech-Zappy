/*
** EPITECH PROJECT, 2023
** Epitech_Zappy
** File description:
** parse_args
*/

#include "args.h"
#include "utility/strings.h"
#include "server.h"
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

const arg_offset_t ARG_OFFSET[] = {
        {'p', offsetof(args_t, port)},
        {'x', offsetof(args_t, width)},
        {'y', offsetof(args_t, height)},
        {'n', offsetof(args_t, names)},
        {'c', offsetof(args_t, slots)},
        {'f', offsetof(args_t, freq)},
        {0, 0}
};

const char *DEFAULT_TEAMS[] = {
        "Team1",
        "Team2",
        "Team3",
        "Team4",
        NULL
};

static void reset_args(args_t *args, bool dup_names)
{
    free_str_array(args->names);
    args->port = DEFAULT_PORT;
    args->width = DEFAULT_WIDTH;
    args->height = DEFAULT_HEIGHT;
    args->names = dup_names ? dupstrarray(DEFAULT_TEAMS) : NULL;
    args->slots = DEFAULT_SLOTS;
    args->freq = DEFAULT_FREQ;
}

static size_t offset_of_arg(int arg)
{
    for (int i = 0; ARG_OFFSET[i].optname; i++)
        if (ARG_OFFSET[i].optname == arg)
            return ARG_OFFSET[i].struct_offset;
    return SIZE_T_MAX;
}

static void fetch_team_names(args_t *args, int ac, char **av)
{
    free_str_array(args->names);
    args->names = my_calloc(sizeof(char *), 1);
    optind--;
    while (optind < ac && av[optind][0] != '-') {
        append_str_array(&args->names, my_strdup(av[optind]));
        optind++;
    }
}

bool check_opt(int opt, args_t *args, int ac, char **av)
{
    if (opt == '?') {
        reset_args(args, false);
        return false;
    }
    if (opt == 'v') {
        *get_is_debug() = true;
    } else if (opt == 'n') {
        fetch_team_names(args, ac, av);
    } else
        *(int *)((char *) args + offset_of_arg(opt)) = atoi(optarg);
    return true;
}

bool get_args(int ac, char **av, args_t *args, char **err)
{
    int opt;

    reset_args(args, true);
    *err = NULL;
    while ((opt = getopt(ac, av, ARGS_STR)) != -1) {
        if (!check_opt(opt, args, ac, av))
            return false;
    }
    if (check_args(args, err))
        return true;
    reset_args(args, false);
    return false;
}
