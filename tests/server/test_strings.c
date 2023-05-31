/*
** EPITECH PROJECT, 2023
** Epitech_Zappy
** File description:
** test_strings
*/

#include "utility/strings.h"
#include "args.h"
#include <criterion/criterion.h>
#include <errno.h>

s_list_t **malloc_stack(void);
#define CHECK_ALL_FREE cr_assert_eq(*malloc_stack(), NULL)

Test(perror_str, perror_str)
{
    errno = 0;
    char *str = perror_str("test");
    cr_assert_str_eq(str, "test: Success");
    cr_assert(errno == 0);
    my_free(str);

    errno = 1;
    str = perror_str("test");
    cr_assert_str_eq(str, "test: Operation not permitted");
    cr_assert(errno == 1);
    my_free(str);

    CHECK_ALL_FREE;
}

Test(append_str_array, append_str_array)
{
    char **arr = NULL;

    append_str_array(&arr, strdup("test"));
    cr_assert_str_eq(arr[0], "test");
    cr_assert_eq(arr[1], NULL);

    append_str_array(&arr, strdup("test2"));
    cr_assert_str_eq(arr[0], "test");
    cr_assert_str_eq(arr[1], "test2");
    cr_assert_eq(arr[2], NULL);

    free_str_array(arr);

    CHECK_ALL_FREE;
}

Test(free_str_array, empty_array)
{
    char **arr = NULL;

    free_str_array(arr);

    CHECK_ALL_FREE;
}

Test(dup_strarray, dup_strarray)
{
    char **arr = NULL;
    char **dup = NULL;

    append_str_array(&arr, strdup("test"));
    append_str_array(&arr, strdup("test2"));
    append_str_array(&arr, strdup("test3"));

    dup = dupstrarray((const char **)arr);

    cr_assert_str_eq(dup[0], "test");
    cr_assert_str_eq(dup[1], "test2");
    cr_assert_str_eq(dup[2], "test3");
    cr_assert_eq(dup[3], NULL);

    free_str_array(arr);
    free_str_array(dup);

    CHECK_ALL_FREE;
}

Test(str_to_word_array, str_to_word_array_get_len)
{
    int len = 0;
    char **arr = str_to_word_array("test test2 test3", " ", &len);

    cr_assert_str_eq(arr[0], "test");
    cr_assert_str_eq(arr[1], "test2");
    cr_assert_str_eq(arr[2], "test3");
    cr_assert_eq(arr[3], NULL);
    cr_assert_eq(len, 3);

    free_str_array(arr);

    CHECK_ALL_FREE;
}

Test(str_to_word_array, str_to_word_array_no_len)
{
    char **arr = str_to_word_array("test test2 test3", " ", NULL);

    cr_assert_str_eq(arr[0], "test");
    cr_assert_str_eq(arr[1], "test2");
    cr_assert_str_eq(arr[2], "test3");
    cr_assert_eq(arr[3], NULL);

    free_str_array(arr);

    CHECK_ALL_FREE;
}

#include <stdio.h>
Test(str_to_word_array, lot_of_cases)
{
    int len = 0;
    char **arr = str_to_word_array("Hello,World", ",", &len);
    cr_assert_str_eq(arr[0], "Hello");
    cr_assert_str_eq(arr[1], "World");
    cr_assert_eq(arr[2], NULL);
    cr_assert_eq(len, 2);
    free_str_array(arr);

    arr = str_to_word_array("Hello,,,World", ",", &len);
    cr_assert_str_eq(arr[0], "Hello");
    cr_assert_str_eq(arr[1], "World");
    cr_assert_eq(arr[2], NULL);
    cr_assert_eq(len, 2);
    free_str_array(arr);


    arr = str_to_word_array("Hello,  World", ", ", &len);
    cr_assert_str_eq(arr[0], "Hello");
    cr_assert_str_eq(arr[1], "World");
    cr_assert_eq(arr[2], NULL);
    cr_assert_eq(len, 2);
    free_str_array(arr);


    arr = str_to_word_array(",Hello,World,,", ",", &len);
    cr_assert_str_eq(arr[0], "Hello");
    cr_assert_str_eq(arr[1], "World");
    cr_assert_eq(arr[2], NULL);
    cr_assert_eq(len, 2);
    free_str_array(arr);


    arr = str_to_word_array("", ",", &len);
    cr_assert_eq(arr[0], NULL);
    cr_assert_eq(len, 0);
    free_str_array(arr);



    arr = str_to_word_array("Hello World", ",", &len);
    cr_assert_str_eq(arr[0], "Hello World");
    cr_assert_eq(arr[1], NULL);
    cr_assert_eq(len, 1);
    free_str_array(arr);


    arr = str_to_word_array("abcdHllo abcdefgh World            ", "abcdefgh ", &len);
    cr_assert_str_eq(arr[0], "Hllo");
    cr_assert_str_eq(arr[1], "Worl");
    cr_assert_eq(arr[2], NULL);
    cr_assert_eq(len, 2);
    free_str_array(arr);

    CHECK_ALL_FREE;
}

Test(split_on, split_on)
{
    int len;
    char **arr = split_on("Hello,World", ",", &len);
    cr_assert_str_eq(arr[0], "Hello");
    cr_assert_str_eq(arr[1], "World");
    cr_assert_eq(len, 2);
    free_str_array(arr);

    arr = split_on(", Hello ,", ", ", NULL);
    cr_assert_str_eq(arr[0], "");
    cr_assert_str_eq(arr[1], "");
    cr_assert_str_eq(arr[2], "Hello");
    cr_assert_str_eq(arr[3], "");
    cr_assert_str_eq(arr[4], "");
    cr_assert_eq(arr[5], NULL);
    free_str_array(arr);

    arr = split_on("Hello", "yyyyyyyyyyyyy", &len);
    cr_assert_str_eq(arr[0], "Hello");
    cr_assert_eq(arr[1], NULL);
    cr_assert_eq(len, 1);
    free_str_array(arr);

    CHECK_ALL_FREE;
}
