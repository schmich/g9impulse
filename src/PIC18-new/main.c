#include "common.h"
#include "delay.h"
#include "intro.h"
#include "sprite.h"
#include "input.h"
#include "background.h"
#include "player.h"
#include "f16.h"
#include "behavior.h"

void playGame(void)
{
    Background* background = createBackground(makeImage(0x002BAD40, 0xA0, 0xF0));
    Player* player = createPlayer(makePoint(80, 80));
    F16* plane = createF16(makePoint(20, -500), 10, createBoring(2));
    F16* plane2 = createF16(makePoint(100, -400), 10, createBoring(1));

    uint8 buffer;

    setDoubleBuffer(true);

    while (player->health > 0)
    {
        drawEntity(background);
        drawEntity(player);
        drawEntity(plane);
        drawEntity(plane2);

        update(plane);
        update(plane2);
        update(background);
        update(player);

        flipBuffer(&buffer);
        delay_ms(16);
    }

    destroy(plane2);
    destroy(plane);
    destroy(player);
    destroy(background);
}
    
void main()
{
    bool showAgain = true;

    memoryInit();
    gpuInit();
    inputInit();
    delay_ms(10);               // wait for system to boot

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
