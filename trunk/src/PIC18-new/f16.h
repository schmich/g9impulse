#ifndef F16_H
#define F16_h

#include "common.h"
#include "enemy.h"

typedef struct F16
{
    ENEMY_BASE;
} F16;

F16* createF16(Point where, uint8 health, Behavior* behavior);

#endif // F16_H
