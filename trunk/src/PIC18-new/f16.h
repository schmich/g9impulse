#ifndef F16_H
#define F16_h

#include "common.h"
#include "enemy.h"

typedef struct F16
{
    //
    // Enemy common base, do not reorder
    //
    uint8      type;
    Point      position;
    Animation* animation;
    uint8      currentFrame;
    uint8      health;
    Behavior   think;
} F16;

F16* createF16(Point where, uint8 health, Behavior behavior);
void destroyF16(F16* who);

void boringF16(Enemy* who);

#endif // F16_H
