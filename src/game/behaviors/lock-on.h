#ifndef LOCKON_H
#define LOCKON_H

#include "common.h"
#include "updateable.h"

typedef struct LockOn
{
    BEHAVIOR_BASE;
    uint16 frequency;
} LockOn;

LockOn* createLockOn(uint16 frequency);

#endif // LOCKON_H
