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

void clear_trailing_newlines(char *str)
{
    int len = strlen(str);

    while (len >= 2 && str[len - 1] == '\n' && str[len - 2] == '\n') {
        str[len - 1] = '\0';
        len--;
    }
}

void debug(const char *format, ...)
{
    va_list ap;
    char *str = NULL;
    size_t tmp;
    char *final_format = str_concat(&tmp, 2, "[DEBUG] ", format);

    if (!*get_is_debug())
        return;
    va_start(ap, format);
    if (vasprintf(&str, final_format, ap) == -1) {
        perror("vasprintf");
        exit(84);
    }
    va_end(ap);
    clear_trailing_newlines(str);
    safe_write(2, str, strlen(str));
    free(str);
}
