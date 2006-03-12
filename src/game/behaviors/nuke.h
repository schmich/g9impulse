#ifndef NUKE_H
#define NUKE_H

#include "common.h"
#include "updateable.h"

typedef struct Nuke
{
    BEHAVIOR_BASE;
    int16 yDetonate;
} Nuke;

Nuke* createNuke(int16 yDetonate);

#endif // NUKE_H
