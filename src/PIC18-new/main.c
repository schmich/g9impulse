#include "common.h"
#include "delay.h"
#include "intro.h"
#include "sprite.h"
#include "input.h"
#include "background.h"
#include "player.h"
#include "f16.h"
#include "behavior.h"
#include "world.h"

void playGame(void)
{
    uint8 buffer = 0;
    Node* curr = NULL;
    Player* player = createPlayer(makePoint(80, 80));
    World* world = createWorld(player);

    setDoubleBuffer(true);
    setFieldColor(0);

    addUpdateable(world, createBackground(makeOpaqueImage(0x002BAD40, 0xA0, 0xF0)));
    addUpdateable(world, player);
    addUpdateable(world, createF16(makePoint(20, -500), 10, createBoring(2)));
    addUpdateable(world, createF16(makePoint(100, -400), 10, createBoring(1)));

    while (player->health > 0)
    {
        for (curr = world->updateables->head; curr != NULL; curr = curr->next)
            drawEntity(curr->data);

        for (curr = world->updateables->head; curr != NULL; curr = curr->next)
            update(curr->data, world);

        flipBuffer(&buffer);
        delay_ms(16);
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
