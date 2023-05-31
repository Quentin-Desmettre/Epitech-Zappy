/*
** EPITECH PROJECT, 2023
** Epitech_Zappy
** File description:
** strings
*/

#ifndef EPITECH_ZAPPY_STRINGS_H
    #define EPITECH_ZAPPY_STRINGS_H
    #include <stddef.h>
    #include <stdbool.h>
    #include <sys/select.h>

// str array
char **str_to_word_array(const char *str, const char *delim, int *len);
void append_str_array(char ***array, char *what);
void free_str_array(char **array);
char **dupstrarray(const char **arr);
char **split_on(char *str, char *delim, int *len);

// Strings
void *memdup(const void *src, size_t size);
char *str_concat_free(size_t *len, int nb_strings, ...);
bool str_ends_with(const char *str, const char *suffix);
void str_append_free(char **str, size_t *len, char *append);
char *str_concat(size_t *len, int nb_strings, ...);

// Other
int bytes_available(int fd);
void get_time(struct timespec *timeout);
int try_select(int fd_max, fd_set *read_fds,
                fd_set *write_fds, struct timeval *timeout);

#endif //EPITECH_ZAPPY_STRINGS_H
