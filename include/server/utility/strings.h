/*
** EPITECH PROJECT, 2023
** Epitech_Zappy
** File description:
** strings
*/

#ifndef EPITECH_ZAPPY_STRINGS_H
    #define EPITECH_ZAPPY_STRINGS_H
    #include <stddef.h>

// str array
char **str_to_word_array(const char *str, const char *delim, int *len);
void append_str_array(char ***array, char *what);
void free_str_array(char **array);
char **dupstrarray(const char * const *arr);

// Strings
void *memdup(const void *src, size_t size);
char *str_concat_free(size_t *len, int nb_strings, ...);
bool str_ends_with(const char *str, const char *suffix);
void str_append_free(char **str, size_t *len, char *append);

// Other
int bytes_available(int fd);
void get_time(struct timespec *timeout);
int try_select(int fd_max, fd_set *read_fds,
               fd_set *write_fds, struct timeval *timeout);

#endif //EPITECH_ZAPPY_STRINGS_H
