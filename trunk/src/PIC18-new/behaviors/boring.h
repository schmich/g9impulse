#ifndef BORING_H
#define BORING_H

#include "common.h"
#include "updateable.h"
#include "fraction.h"

typedef struct Boring
{
    BEHAVIOR_BASE;
    Fraction vertSpeed;
    int8 counter;
} Boring;

Boring* createBoring(Fraction vertSpeed);

#endif // BORING_H
