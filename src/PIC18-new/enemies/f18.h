#ifndef F18_H
#define F18_h

#include "common.h"
#include "enemy.h"

typedef struct F18
{
    ENEMY_BASE;
} F18;

F18* createF18(int16 x, int16 recess, uint8 health, Behavior* behavior);

#endif // F18_H
