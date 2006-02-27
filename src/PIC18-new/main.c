#include "common.h"
#include "intro.h"
#include "delay.h"
#include "input.h"
#include "player.h"
#include "world.h"
#include "level1.h"

void playGame(void)
{
    uint8 buffer = 0;
    World* world = createWorld(createPlayer(makePoint(80, 200)), createLevel1());

    setDoubleBuffer(true);
    setFieldColor(0);

    flipBuffer(&buffer);

    while (active(world))
    {
        drawWorld(world);
        updateWorld(world);
        collideWorld(world);

        flipBuffer(&buffer);
        delay_ms(10);
    }

    destroy(world);
}
    
void main()
{
    bool showAgain = true;

    memoryInit();
    gpuInit();
    inputInit();
    delay_ms(10);               // wait for system to boot

    setFieldColor(0);

#ifndef _DEBUG
    showBootSplash();

    while (showAgain)
    {
        showIntro();
        showAgain = showTitle();
    }
#endif

    //
    // until the end of time (or major disaster)
    //
    while (true)
    {        
        playGame();

        while (showTitle())
            showIntro();
    }
}
