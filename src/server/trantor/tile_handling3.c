/*
** EPITECH PROJECT, 2023
** Epitech_Zappy
** File description:
** tile_handling
*/

#include "trantor.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "utility/garbage_collector.h"

int get_direct_from_angle(double angle)
{
    if (angle >= 0 && angle < 45)
        return 1;
    if (angle == 45)
        return 8;
    if (angle > 45 && angle < 135)
        return 7;
    if (angle == 135)
        return 6;
    if (angle > 135 && angle < 225)
        return 5;
    if (angle == 225)
        return 4;
    if (angle > 225 && angle < 315)
        return 3;
    if (angle == 315)
        return 2;
    return 1;
}
