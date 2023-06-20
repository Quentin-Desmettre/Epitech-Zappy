/*
** EPITECH PROJECT, 2023
** Epitech-Zappy
** File description:
** debug_print
*/

#include "server.h"
#include "utility/strings.h"
#include <stdarg.h>
#include "utility/safe_write.h"
#include "trantor.h"

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

char *get_debug_time_str(void)
{
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);

    return my_asprintf("[DEBUG %02d:%02d:%02d.%03d ms ] ",
        tm->tm_hour, tm->tm_min, tm->tm_sec,
        (int)(clock() * 1000 / CLOCKS_PER_SEC));
}

void debug(const char *format, ...)
{
    va_list ap;
    char *formated = NULL;
    char *final_str;

    if (!*get_is_debug())
        return;
    va_start(ap, format);
    if (vasprintf(&formated, format, ap) == -1) {
        perror("vasprintf");
        exit(84);
    }
    va_end(ap);
    final_str = get_debug_time_str();
    str_append(&final_str, formated);
    free(formated);
    clear_trailing_newlines(final_str);
    safe_write(2, final_str, strlen(final_str));
}
