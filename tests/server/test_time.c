/*
** EPITECH PROJECT, 2023
** Epitech-Zappy
** File description:
** test_time
*/

#include "server.h"
#include <criterion/criterion.h>
#include "utility/strings.h"

#define ABS(x) ((x) < 0 ? -(x) : (x))
#define NS_DELTA 1
#define IS_NS_EQ(a, b) (ABS((a) - (b)) <= NS_DELTA)

#define FLOAT_DELTA 0.00001
#define IS_FLOAT_EQ(a, b) (ABS((a) - (b)) <= FLOAT_DELTA)

Test(get_end_time, whole_ticks)
{
    struct timespec now = {0, 0};
    struct timespec end = get_end_time(10, 5, now);
    cr_assert(end.tv_sec == 2);
    cr_assert(end.tv_nsec == 0);

    end = get_end_time(7, 5, now);
    cr_assert(end.tv_sec == 1);

    end = get_end_time(6, 5, now);
    cr_assert(IS_NS_EQ(end.tv_nsec, 200000000), "actual = %ld, expected = %d", end.tv_nsec, 400000000);

    end = get_end_time(5, 5, now);
    cr_assert(end.tv_sec == 1);
    cr_assert(end.tv_nsec == 0);

    end = get_end_time(2, 5, now);
    cr_assert(end.tv_sec == 0);
    cr_assert(IS_NS_EQ(end.tv_nsec, 400000000));

    end = get_end_time(1, 5, now);
    cr_assert(end.tv_sec == 0);
    cr_assert(IS_NS_EQ(end.tv_nsec, 200000000));
}

Test(get_end_time, float_ticks)
{
    struct timespec now = {0, 0};
    struct timespec end;

    end = get_end_time(2.7, 5, now);
    cr_assert(end.tv_sec == 0);
    cr_assert(IS_NS_EQ(end.tv_nsec, 540000000), "actual = %ld, expected = %d", end.tv_nsec, 540000000);

    end = get_end_time(1.7, 5, now);
    cr_assert(end.tv_sec == 0);
    cr_assert(IS_NS_EQ(end.tv_nsec, 340000000), "actual = %ld, expected = %d", end.tv_nsec, 340000000);


    end = get_end_time(33.876, 5, now);
    cr_assert(end.tv_sec == 6);
    cr_assert(IS_NS_EQ(end.tv_nsec, 775200000), "actual = %ld, expected = %d", end.tv_nsec, 775200000);

    end = get_end_time(0.876, 5, now);
    cr_assert(end.tv_sec == 0);
    cr_assert(IS_NS_EQ(end.tv_nsec, 175200000), "actual = %ld, expected = %d", end.tv_nsec, 175200000);
}

Test(get_action_progress, now_at_zero)
{
    action_t ac = {
            .start_time = {0, 0},
            .end_time = {1, 0},
    };
    struct timespec now;

    now = (struct timespec){0, 0};
    cr_assert(IS_FLOAT_EQ(get_action_progress(&ac, now), 0));

    now = (struct timespec){0, 0.2 * S_TO_NS};
    cr_assert(IS_FLOAT_EQ(get_action_progress(&ac, now), 0.2));

    now = (struct timespec){0, 0.5 * S_TO_NS};
    cr_assert(IS_FLOAT_EQ(get_action_progress(&ac, now), 0.5));

    now = (struct timespec){0, 0.9 * S_TO_NS};
    cr_assert(IS_FLOAT_EQ(get_action_progress(&ac, now), 0.9));

    now = (struct timespec){1, 0};
    cr_assert(IS_FLOAT_EQ(get_action_progress(&ac, now), 1));
}

Test(get_action_progress, now_at_random_place)
{
    struct timespec now = {
            .tv_sec = 5,
            .tv_nsec = 123456789,
    };
    action_t ac = {
            .start_time = {4, 0},
            .end_time = {6, 0}
    };

    // Action span: 6 - 4 = 2
    // Action already done: 5.123456789 - 4 = 1.123456789
    cr_assert(IS_FLOAT_EQ(get_action_progress(&ac, now), 1.123456789 / 2));

    ac.start_time = (struct timespec){4, 987654321};
    // Action span: 6 - 4.987654321 = 1.012345679
    // Action already done: 5.123456789 - 4.987654321 = 0.135802468
    cr_assert(IS_FLOAT_EQ(get_action_progress(&ac, now), 0.135802468 / 1.012345679));
}
