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
    #define NULL_IF_NEG(x) ((x) & ~((x) >> (sizeof(int) * 8 - 1)))

// str array
char **str_to_word_array(const char *str, const char *delim, int *len);
void append_str_array(char ***array, char *what);
void free_str_array(char **array);
char **dupstrarray(const char **arr);
char **split_on(char *str, char *delim, int *len);
bool has_duplicates(char **arr);
int str_array_len(char **array);
bool str_array_contains(char **arr, char *str);

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
bool is_timespec_less(struct timespec *t1, struct timespec *t2);
bool is_timespec_equal(struct timespec *t1, struct timespec *t2);
struct timespec get_end_time(int ticks, int freq, struct timespec now);
struct timeval timespec_diff(struct timespec a, struct timespec b);
char *my_asprintf(const char *format, ...);
void str_append(char **str, char *append);

#endif //EPITECH_ZAPPY_STRINGS_H
