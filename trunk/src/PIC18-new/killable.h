#ifndef KILLABLE_H
#define KILLABLE_H

#include "common.h"
#include "updateable.h"

#define KILLABLE_BASE \
    UPDATEABLE_BASE; \
    uint8 health;

typedef struct Killable
{
    KILLABLE_BASE;
} Killable;

#endif // KILLABLE_H
