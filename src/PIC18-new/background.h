#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "common.h"
#include "updateable.h"

typedef struct Background
{
    UPDATEABLE_BASE;
} Background;

Background* createBackground(Image image);

#endif // BACKGROUND_H
