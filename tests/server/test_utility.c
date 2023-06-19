/*
** EPITECH PROJECT, 2023
** Epitech_Zappy
** File description:
** test_utility
*/

#include "utility/garbage_collector.h"
#include <criterion/criterion.h>
#include <stdio.h>
#include <criterion/redirect.h>

static void redirect_stdout_err(void)
{
    cr_redirect_stderr();
    cr_redirect_stdout();
}
s_list_t **malloc_stack(void);

Test(my_malloc, my_malloc_normal)
{
    char *str = my_malloc(10);
    s_list_t *tmp;

    cr_assert_not_null(str);
    cr_assert(str == (*malloc_stack())->data);
    cr_assert((*malloc_stack())->next == NULL);
    tmp = *malloc_stack();
    str = my_malloc(10);
    cr_assert(str == (*malloc_stack())->data);
    cr_assert((*malloc_stack())->next == tmp);
    cr_assert(tmp->next == NULL);
    my_free(tmp->data);
    my_free(str);
    cr_assert(*malloc_stack() == NULL);
}

Test(my_malloc, my_calloc_normal)
{
    char *str = my_calloc(10, 10);
    s_list_t *tmp;

    cr_assert_not_null(str);
    for (int i = 0; i < 10; i++)
        cr_assert(str[i] == 0);
    cr_assert(str == (*malloc_stack())->data);
    cr_assert((*malloc_stack())->next == NULL);
    tmp = *malloc_stack();
    str = my_calloc(10, 10);
    cr_assert(str == (*malloc_stack())->data);
    cr_assert((*malloc_stack())->next == tmp);
    cr_assert(tmp->next == NULL);
    my_free(tmp->data);
    my_free(str);
    cr_assert(*malloc_stack() == NULL);
}

Test(my_malloc, my_realloc_normal)
{
    char *str = my_malloc(10);

    cr_assert_not_null(str);
    cr_assert(str == (*malloc_stack())->data);
    cr_assert((*malloc_stack())->next == NULL);
    str = my_realloc(str, 20);
    cr_assert_not_null(str);
    cr_assert(str == (*malloc_stack())->data);
    cr_assert((*malloc_stack())->next == NULL);
}

Test(my_malloc, my_realloc_null)
{
    char *str = my_realloc(NULL, 20);

    cr_assert_not_null(str);
    cr_assert(str == (*malloc_stack())->data);
    cr_assert((*malloc_stack())->next == NULL);
}

Test(my_malloc, my_strdup_normal)
{
    char *str = my_strdup("Hello World");
    s_list_t *tmp;

    cr_assert_not_null(str);
    cr_assert(str == (*malloc_stack())->data);
    cr_assert((*malloc_stack())->next == NULL);
    tmp = *malloc_stack();
    str = my_strdup("Hello World");
    cr_assert(str == (*malloc_stack())->data);
    cr_assert((*malloc_stack())->next == tmp);
    cr_assert(tmp->next == NULL);
    my_free(tmp->data);
    my_free(str);
    cr_assert(*malloc_stack() == NULL);
}

Test(my_malloc, enormous_malloc, .exit_code = 84, .init = redirect_stdout_err)
{
    my_malloc(1000000000000);
}

Test(my_malloc, enormous_calloc, .exit_code = 84, .init = redirect_stdout_err)
{
    my_calloc(1000000000000, 1000000000000);
}

Test(my_free, no_such_ptr)
{
    my_free(NULL);
}
