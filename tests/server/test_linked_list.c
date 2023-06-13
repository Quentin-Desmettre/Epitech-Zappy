/*
** EPITECH PROJECT, 2023
** Epitech_Zappy
** File description:
** test_linked_list
*/

#include "utility/linked_list.h"
#include <criterion/criterion.h>
#include "utility/garbage_collector.h"

#define CHECK_ALL_FREE cr_assert_eq(*malloc_stack(), NULL)

s_list_t **malloc_stack(void);

Test(append_node, append_node)
{
    list_t *list = NULL;

    append_node(&list, strdup("test"));
    cr_assert_str_eq(list->data, "test");
    cr_assert_eq(list->next, list);
    cr_assert_eq(list->prev, list);

    append_node(&list, strdup("test2"));
    cr_assert_str_eq(list->data, "test");
    cr_assert_str_eq(list->next->data, "test2");
    cr_assert_str_eq(list->prev->data, "test2");
    cr_assert_eq(list->next->next, list);
    cr_assert_eq(list->prev->prev, list);
    cr_assert_eq(list->next->prev, list);
    cr_assert_eq(list->prev->next, list);

    free_list(&list, free);

    CHECK_ALL_FREE;
}

Test(push_to_front, push_to_front)
{
    list_t *list = NULL;

    append_node(&list, strdup("test"));
    cr_assert_str_eq(list->data, "test");
    cr_assert_eq(list->next, list);
    cr_assert_eq(list->prev, list);

    push_to_front(&list, strdup("test2"));
    cr_assert_str_eq(list->data, "test2");
    cr_assert_str_eq(list->next->data, "test");
    cr_assert_str_eq(list->prev->data, "test");
    cr_assert_eq(list->next->next, list);
    cr_assert_eq(list->prev->prev, list);
    cr_assert_eq(list->next->prev, list);
    cr_assert_eq(list->prev->next, list);

    free_list(&list, free);

    CHECK_ALL_FREE;
}

Test(list_size, list_size)
{
    list_t *list = NULL;

    cr_assert_eq(list_size(list), 0);
    append_node(&list, strdup("test"));
    cr_assert_eq(list_size(list), 1);
    append_node(&list, strdup("test2"));
    cr_assert_eq(list_size(list), 2);
    append_node(&list, strdup("test3"));
    cr_assert_eq(list_size(list), 3);
    append_node(&list, strdup("test4"));
    cr_assert_eq(list_size(list), 4);
    append_node(&list, strdup("test5"));
    cr_assert_eq(list_size(list), 5);
    append_node(&list, strdup("test6"));
    cr_assert_eq(list_size(list), 6);

    free_list(&list, free);
    CHECK_ALL_FREE;
}

Test(remove_node, remove_node)
{
    list_t *list = NULL;

    remove_node(&list, 12, NULL);
    append_node(&list, strdup("test"));
    append_node(&list, strdup("test2"));
    remove_node(&list, 1, free);
    cr_assert_str_eq(list->data, "test");
    cr_assert_eq(list->next, list);
    cr_assert_eq(list->prev, list);
    remove_node(&list, 0, NULL);
    cr_assert_eq(list, NULL);

    CHECK_ALL_FREE;
}

Test(remove_if, remove_if_no_comparator)
{
    list_t *list = NULL;

    append_node(&list, "test");
    append_node(&list, "test2");
    append_node(&list, "test3");
    append_node(&list, "test4");
    remove_if(&list, "test", NULL, NULL);
    cr_assert_str_eq(list->data, "test2");
    cr_assert_str_eq(list->next->data, "test3");
    cr_assert_str_eq(list->next->next->data, "test4");
    remove_if(&list, "test2", NULL, NULL);
    remove_if(&list, "test3", NULL, NULL);
    remove_if(&list, "test4", NULL, NULL);

    CHECK_ALL_FREE;
}

bool eq_cmp(void *data, void *data2)
{
    return !strcmp(data, data2);
}

Test(remove_if, remove_if_comparator)
{
    list_t *list = NULL;

    append_node(&list, "test2");
    append_node(&list, "test");
    remove_if(&list, "test", eq_cmp, NULL);
    cr_assert_str_eq(list->data, "test2");
    cr_assert_eq(list->next, list);
    cr_assert_eq(list->prev, list);
    remove_if(&list, "test2", eq_cmp, NULL);
    cr_assert_eq(list, NULL);

    CHECK_ALL_FREE;
}

Test(remove_if, remove_if_empty_list)
{
    list_t *list = NULL;

    remove_if(&list, NULL, NULL, NULL);
    cr_assert_eq(list, NULL);

    CHECK_ALL_FREE;
}

Test(remove_if, remove_if_no_such_elem)
{
    list_t *list = NULL;

    append_node(&list, "test");
    append_node(&list, "test");
    remove_if(&list, "test2", NULL, NULL);
    cr_assert_str_eq(list->data, "test");
    cr_assert_str_eq(list->next->data, "test");
    cr_assert_eq(list->next->next, list);
    cr_assert_eq(list->prev->prev, list);
    cr_assert_neq(list->next, list);
    cr_assert_neq(list->prev, list);

    free_list(&list, NULL);

    CHECK_ALL_FREE;
}

Test(remove_if, multiple_matching_nodes)
{
    list_t *list = NULL;

    append_node(&list, "test");
    append_node(&list, "test");
    append_node(&list, "test");
    append_node(&list, "test");
    append_node(&list, "test2");
    append_node(&list, "test2");
    remove_if(&list, "test", eq_cmp, NULL);
    cr_assert_str_eq(list->data, "test2");
    cr_assert_str_eq(list->next->data, "test2");
    cr_assert_neq(list->next, list);
    remove_if(&list, "test2", eq_cmp, NULL);
    cr_assert_eq(list, NULL);

    CHECK_ALL_FREE;
}

Test(append_list, empty_first_list)
{
    list_t *list = NULL;
    list_t *list2 = NULL;

    append_list(&list, list2, false, NULL);
    cr_assert_eq(list, NULL);
    CHECK_ALL_FREE;
}

Test(append_list, empty_second_list)
{
    list_t *list = NULL;
    list_t *list2 = NULL;

    append_node(&list2, "test");
    append_list(&list, list2, true, NULL);
    cr_assert_str_eq(list->data, "test");
    cr_assert_eq(list->next, list);
    cr_assert_eq(list->prev, list);
    free_list(&list, NULL);

    CHECK_ALL_FREE;
}

Test(append_list, append_list_free)
{
    list_t *list = NULL;
    list_t *list2 = NULL;

    append_node(&list, "test");
    append_node(&list, "test2");
    append_node(&list, "test3");
    append_node(&list, "test4");
    append_list(&list2, list, 1, NULL);
    cr_assert_str_eq(list2->data, "test");
    cr_assert_str_eq(list2->next->data, "test2");
    cr_assert_str_eq(list2->next->next->data, "test3");
    cr_assert_str_eq(list2->next->next->next->data, "test4");
    free_list(&list2, NULL);

    CHECK_ALL_FREE;
}

Test(append_list, append_list_no_free)
{
    list_t *list = NULL;
    list_t *list2 = NULL;

    append_node(&list, "test");
    append_node(&list, "test2");
    append_node(&list, "test3");
    append_node(&list, "test4");
    append_list(&list2, list, 0, NULL);

    // List 2
    cr_assert_str_eq(list2->data, "test");
    cr_assert_str_eq(list2->next->data, "test2");
    cr_assert_str_eq(list2->next->next->data, "test3");
    cr_assert_str_eq(list2->next->next->next->data, "test4");

    // List 1 untouched
    cr_assert_str_eq(list->data, "test");
    cr_assert_str_eq(list->next->data, "test2");
    cr_assert_str_eq(list->next->next->data, "test3");
    cr_assert_str_eq(list->next->next->next->data, "test4");

    free_list(&list2, NULL);
    free_list(&list, NULL);

    CHECK_ALL_FREE;
}
