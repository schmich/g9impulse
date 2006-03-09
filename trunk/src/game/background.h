#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "common.h"
#include "updateable.h"

typedef struct Background
{
    UPDATEABLE_BASE;
    uint16 progress;
    uint8  step;
    uint16 offset;
} Background;

Background* createBackground(void);
void drawBackground(Background* bg);

#endif // BACKGROUND_H
