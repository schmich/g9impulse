#ifndef ENEMY_H
#define ENEMY_H

#include "common.h"
#include "killable.h"

#define ENEMY_BASE \
    KILLABLE_BASE;

typedef struct Enemy
{
    ENEMY_BASE;
} Enemy;

#endif // ENEMY_H
