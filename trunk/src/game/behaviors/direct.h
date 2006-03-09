#ifndef DIRECT_H
#define DIRECT_H

#include "common.h"
#include "updateable.h"

typedef struct Direct
{
    BEHAVIOR_BASE;
    Point start;
    Point target;
    uint8 speed;
    int16 error;
} Direct;

Direct* createDirect(Point start, Point target, uint8 speed);

#endif // DIRECT_H
