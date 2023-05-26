/*
** EPITECH PROJECT, 2023
** Epitech_Zappy
** File description:
** args
*/

#ifndef EPITECH_ZAPPY_ARGS_H
    #define EPITECH_ZAPPY_ARGS_H
    #include <stddef.h>
    #include <stdbool.h>

    // Defaults
    #define DEFAULT_PORT 4242
    #define DEFAULT_FREQ 100
    #define DEFAULT_WIDTH 10
    #define DEFAULT_HEIGHT 10
    #define DEFAULT_SLOTS 3
    #define ARGS_STR "p:x:y:n:c:f:"
    #define SIZE_T_MAX ((size_t)-1)
extern const char *DEFAULT_TEAMS[];

    // Errors
    #define ERR_PORT "Error: invalid port"
    #define ERR_FREQ "Error: invalid frequency"
    #define ERR_WIDTH "Error: width must be in the interval [10, 30]"
    #define ERR_HEIGHT "Error: height must be in the interval [10, 30]"
    #define ERR_TEAMS "Error: invalid team names"
    #define ERR_SLOTS "Error: invalid slots"

typedef struct {
    char optname;
    size_t struct_offset;
} arg_offset_t;

typedef struct args {
    int port;
    int width;
    int height;
    char **names;
    int slots;
    int freq;
} args_t;

extern const arg_offset_t ARG_OFFSET[];

bool get_args(int ac, char **av, args_t *args, char **err);
char *perror_str(const char *str);

#endif //EPITECH_ZAPPY_ARGS_H
