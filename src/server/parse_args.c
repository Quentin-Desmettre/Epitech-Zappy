/*
** EPITECH PROJECT, 2023
** Epitech_Zappy
** File description:
** parse_args
*/

#include "args.h"
#include "strings.h"
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

static void reset_args(args_t *args)
{
    free_str_array(args->names);
    args->port = DEFAULT_PORT;
    args->width = DEFAULT_WIDTH;
    args->height = DEFAULT_HEIGHT;
    args->names = dupstrarray(DEFAULT_TEAMS);
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

static bool fetch_team_names(args_t *args, int ac, char **av, char **err)
{
    if (optarg == NULL) {
        *err = "Missing team names";
        return false;
    }
    free_str_array(args->names);
    args->names = my_calloc(1, sizeof(char *));
    optind--;
    while (optind < ac && av[optind][0] != '-') {
        append_str_array(&args->names, my_strdup(av[optind]));
        optind++;
    }
    return true;
}

static bool check_args(args_t *args, char **err)
{
    if (args->port <= 0)
        *err = ERR_PORT;
    if (args->freq <= 0)
        *err = ERR_FREQ;
    if (args->width < 10 || args->width > 30)
        *err = ERR_WIDTH;
    if (args->height < 10 || args->height > 30)
        *err = ERR_HEIGHT;
    if (args->slots <= 0)
        *err = ERR_SLOTS;
    if (args->names == NULL || args->names[0] == NULL)
        *err = ERR_TEAMS;
    return *err == NULL;
}

bool get_args(int ac, char **av, args_t *args, char **err)
{
    int opt;

    reset_args(args);
    *err = NULL;
    while ((opt = getopt(ac, av, ARGS_STR)) != -1) {
        if (opt == '?')
            return false;
        if (opt == 'n' && !fetch_team_names(args, ac, av, err))
            return false;
        if (opt != 'n')
            *(int *)((char *) args + offset_of_arg(opt)) = atoi(optarg);
    }
    return check_args(args, err);
}
