#ifndef POINT_H
#define POINT_H

#include "common.h"

typedef struct Point
{
    int16 x;
    int16 y;
} Point;

Point makePoint(int16 x, int16 y);

#endif // POINT_H
