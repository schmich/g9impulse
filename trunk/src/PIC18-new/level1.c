#include "level1.h"
#include "f16.h"
#include "behavior.h"
#include "cloud.h"

static void spawnTriple(World* w, int16 x)
{
    addEnemy(w, createF16(x, 0, 2, createBoring(2))); 
    addEnemy(w, createF16(x + 15, 40, 2, createBoring(2))); 
    addEnemy(w, createF16(x + 30, 80, 2, createBoring(2))); 
}

#include "level1.lvl.inl"
