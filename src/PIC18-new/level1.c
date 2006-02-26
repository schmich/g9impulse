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

static void spawn(World* w, uint16 time)
{
    switch (time)
    {
        case 300:
            addUnderlay(w, createCloud(-30, 0, 1, CLOUD_LARGE));
            addEnemy(w, createF16(10, 0, 2, createBoring(3)));
            addEnemy(w, createF16(90, 0, 2, createBoring(1)));
            break;

        case 500:
            addEnemy(w, createF16(20, 0, 2, createBoring(2)));
            addEnemy(w, createF16(140, 0, 2, createBoring(3)));
            addOverlay(w, createCloud(20, 0, 1, CLOUD_TINY));
            addOverlay(w, createCloud(80, 30, 1, CLOUD_MEDIUM));
            break;

        case 750:
            spawnTriple(w, 60);
            addUnderlay(w, createCloud(90, 0, 1, CLOUD_SMALL));
            addOverlay(w, createCloud(10, 20, 1, CLOUD_MEDIUM));
            break;
    }
}

Level theLevel1 = 
{
    NULL, spawn, 0, 
    {
        300,
        500,
        750,
        0
    }
};

bool theLevel1Init = false;

Level* createLevel1(void)
{
    if (!theLevel1Init)
    {
        theLevel1.background = createBackground(makeOpaqueImage(0x002C4340, 0xA0, 0xF0));
        theLevel1Init = true;
    }

    return &theLevel1;
}
