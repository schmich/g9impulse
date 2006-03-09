#ifndef HOVER_H
#define HOVER_H

#include "common.h"
#include "updateable.h"

typedef struct Hover
{
    BEHAVIOR_BASE;
    uint16 duration;
    int16  yMin;
    bool   retreat;
} Hover;

Hover* createHover(int16 yMin, uint16 duration);

#endif // HOVER_H
