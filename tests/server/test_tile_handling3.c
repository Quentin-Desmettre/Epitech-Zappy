/*
** EPITECH PROJECT, 2023
** Epitech-Zappy
** File description:
** test_handle_actions
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "server.h"
#include "utility/strings.h"

Test(get_direct_from_angle, testing_different_directions)
{
    cr_assert_eq(get_direct_from_angle(0), 1);
    cr_assert_eq(get_direct_from_angle(45), 8);
    cr_assert_eq(get_direct_from_angle(90), 7);
    cr_assert_eq(get_direct_from_angle(135), 6);
    cr_assert_eq(get_direct_from_angle(180), 5);
    cr_assert_eq(get_direct_from_angle(225), 4);
    cr_assert_eq(get_direct_from_angle(270), 3);
    cr_assert_eq(get_direct_from_angle(315), 2);
    cr_assert_eq(get_direct_from_angle(360), 1);
}
