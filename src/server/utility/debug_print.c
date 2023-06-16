/*
** EPITECH PROJECT, 2023
** Epitech-Zappy
** File description:
** debug_print
*/

#include "utility/strings.h"
#include <stdarg.h>
#include "utility/safe_write.h"

void debug(const char *format, ...)
{
    va_list ap;
    char *str = NULL;
    char *dup;

    va_start(ap, format);
    if (vasprintf(&str, format, ap) == -1) {
        perror("vasprintf");
        exit(84);
    }
    va_end(ap);
    safe_write(2, str, strlen(str));
    free(str);
}
