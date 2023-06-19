/*
** EPITECH PROJECT, 2023
** Epitech_Zappy
** File description:
** str_to_word_array
*/

#include "utility/garbage_collector.h"
#include <string.h>
#include <stdbool.h>

bool contain(char const *str, char c)
{
    while (*str) {
        if (*str == c)
            return true;
        str++;
    }
    return false;
}

static int count_words(char const *str, char const *delimiters)
{
    int counter = 0;
    int checker = 0;
    int increment = 1;

    while (*str) {
        if (!checker && !contain(delimiters, *str)) {
            checker = 1;
            counter++;
        }
        increment = 1;
        while (*str && contain(delimiters, *str)) {
            str++;
            checker = 0;
            increment = 0;
        }
        if (increment)
            str++;
    }
    return counter;
}

static int get_word_length(char const *str, char const *delimiters)
{
    int len = 0;

    while (str[len] && !contain(delimiters, str[len]))
        len++;
    return len;
}

static char *get_word(char const *str, char const *delimiters)
{
    int length = get_word_length(str, delimiters);
    char *word = my_malloc(sizeof(char) * (length + 1));

    strncpy(word, str, length);
    word[length] = '\0';
    return word;
}

char **str_to_word_array(char const *str, char const *delimiters, int *len)
{
    int word_count = count_words(str, delimiters);
    char **array = malloc(sizeof(char *) * (word_count + 1));

    if (len)
        *len = word_count;
    while (*str && contain(delimiters, *str))
        str++;
    for (int index = 0; index < word_count; index++) {
        array[index] = get_word(str, delimiters);
        str += strlen(array[index]);
        while (*str && contain(delimiters, *str))
            str++;
    }
    array[word_count] = 0;
    return array;
}
