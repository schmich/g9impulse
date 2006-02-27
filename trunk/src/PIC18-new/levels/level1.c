#include "level1.h"
#include "f16.h"
#include "f18.h"
#include "boring.h"
#include "cloud.h"
#include "chase.h"

static void spawnTripleF16(World* w, int16 x)
{
    addEnemy(w, createF16(x, 0, 2, createBoring(2))); 
    addEnemy(w, createF16(x + 15, 40, 2, createBoring(2))); 
    addEnemy(w, createF16(x + 30, 80, 2, createBoring(2))); 
}

static void spawnManyF18(World* w, int16 x, bool increasing)
{
    int8 mult;
    if (increasing)
        mult = 1;
    else
        mult = -1;

    addEnemy(w, createF18(x + mult * 10, 20, 1, createChase(2, 1))); 
    addEnemy(w, createF18(x + mult * 20, 40, 1, createChase(2, 1))); 
    addEnemy(w, createF18(x + mult * 30, 60, 1, createChase(2, 1))); 
    addEnemy(w, createF18(x + mult * 40, 80, 1, createChase(2, 1))); 
}

#include "level1.lvl.inl"
