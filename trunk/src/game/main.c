#include "common.h"
#include "intro.h"
#include "delay.h"
#include "input.h"
#include "player.h"
#include "world.h"
#include "level1.h"
#include "timer.h"
#include "pause.anim.inl"
#include "gpu.h"
#include "spu.h"

#define FRAME_TIME 900

void handleInput(uint8* buffer)
{
    Animation* pause = pauseAnimation();

    if (getInputStatus()->startPressed)
    {
#ifndef _DEBUG
        drawImage(&pause->images[0],
                  makePoint(60, 85),
                  true);

        drawImage(&pause->images[1],
                  makePoint(6, 10),
                  true);
#endif

        flipBuffer(buffer);

        while (getInputStatus()->startPressed) ;
        while (!getInputStatus()->startPressed) ;

        getInputStatus()->startPressed = false;
    }
}

void playGame(void)
{
    uint8 buffer = 0;
    World* world = createWorld(createPlayer(makePoint(0, 0)), createLevel1());

    setDoubleBuffer(true);
    clearBuffers();
    setFieldColor(0);

    initTimer();

    playMusic(2);

    //
    // HACK prevent immediate pause (from title screen)
    //
    getInputStatus()->startPressed = false;

    while (active(world))
    {
        resetTimer();

        drawWorld(world);
        updateWorld(world);
        collideWorld(world);

        handleInput(&buffer);

        //
        // HACK drawing bug, uncomment to exploit
        //
        //draw(0x000D97B2, 2, 6, 87, 1, true);
        
        flipBuffer(&buffer);

        while (timeElapsed() < FRAME_TIME)
        {
            //
            // hang out until we've hit frame target time
            //
        }
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
