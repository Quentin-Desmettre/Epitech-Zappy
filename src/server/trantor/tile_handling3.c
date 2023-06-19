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
    if (angle >= 22.5 && angle < 67.5)
        return 8;
    if (angle >= 67.5 && angle < 112.5)
        return 7;
    if (angle >= 112.5 && angle < 157.5)
        return 6;
    if (angle >= 157.5 && angle < 202.5)
        return 5;
    if (angle >= 202.5 && angle < 247.5)
        return 4;
    if (angle >= 247.5 && angle < 292.5)
        return 3;
    if (angle >= 292.5 && angle < 337.5)
        return 2;
    return 1;
}
