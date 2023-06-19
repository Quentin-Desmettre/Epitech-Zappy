/*
** EPITECH PROJECT, 2023
** Epitech-Zappy
** File description:
** debug_print
*/

#include "server.h"
#include "utility/strings.h"
#include <stdarg.h>

bool *get_is_debug(void)
{
    static bool is_debug = false;

    return &is_debug;
}

void debug(const char *format, ...)
{
    va_list ap;
    char *str = NULL;

    if (!*get_is_debug())
        return;
    va_start(ap, format);
    if (vasprintf(&str, format, ap) == -1) {
        perror("vasprintf");
        exit(84);
    }
    va_end(ap);
    safe_write(2, str, strlen(str));
    free(str);
}
