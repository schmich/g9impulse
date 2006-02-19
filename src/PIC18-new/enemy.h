#ifndef ENEMY_H
#define ENEMY_H

#include "common.h"
#include "entity.h"

struct Enemy;
typedef far rom void (*Behavior)(struct Enemy*);

typedef struct Enemy
{
    //
    // Entity common base, do not reorder
    //
    uint8      type;
    Point      position;
    Animation* animation;
    uint8      currentFrame;

    //
    // Enemy specific data
    //
    uint8    health;
    Behavior think;
} Enemy;

void thinkEnemy(Enemy* who);

#endif // ENEMY_H
