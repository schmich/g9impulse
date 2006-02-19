#include "common.h"
#include "delay.h"
#include "intro.h"
#include "sprite.h"
#include "input.h"
#include "player.h"
#include "f16.h"

#define PLAYER_MAX_MOMENTUM_Y 13
#define PLAYER_MAX_MOMENTUM_X 10

void scrollBackground(Image* background)
{
    background->address -= background->width;
}

void handlePlayerInput(Player* player)
{
    Input* input = getInputStatus();
    uint8 width;
    uint8 height;

    if (input->leftPressed)
    {
        player->position.x -= 1;

        if (--player->momentum.x < -(PLAYER_MAX_MOMENTUM_X / 2))
        {
            if (player->momentum.x < -PLAYER_MAX_MOMENTUM_X)
                player->momentum.x = -PLAYER_MAX_MOMENTUM_X;

            rollLeft(player);
        }
    }
    else if (input->rightPressed)
    {
        player->position.x += 1;

        if (++player->momentum.x > (PLAYER_MAX_MOMENTUM_X / 2))
        {
            if (player->momentum.x > PLAYER_MAX_MOMENTUM_X)
                player->momentum.x = PLAYER_MAX_MOMENTUM_X;

            rollRight(player);
        }
    }
    else
    {
        if (player->momentum.x < 0)
            ++player->momentum.x;
        else if (player->momentum.x > 0)
            --player->momentum.x;

        if ((player->momentum.x > -(PLAYER_MAX_MOMENTUM_X / 2)) &&
            (player->momentum.x < (PLAYER_MAX_MOMENTUM_X / 2)))
            noRoll(player);
    }

    if (input->upPressed)
    {
        if (--player->momentum.y < -PLAYER_MAX_MOMENTUM_Y)
            player->momentum.y = -PLAYER_MAX_MOMENTUM_Y;
    }
    else if (input->downPressed)
    {
        if (++player->momentum.y > PLAYER_MAX_MOMENTUM_Y)
            player->momentum.y = PLAYER_MAX_MOMENTUM_Y;
    }
    else
    {
        if (player->momentum.y < 0)
            ++player->momentum.y;
        else if (player->momentum.y > 0)
            --player->momentum.y;
    }

    player->position.x += player->momentum.x / 4;
    player->position.y += player->momentum.y / 3;

    if (player->position.x < 0)
    {
        player->position.x = 0;
    }
    else
    {
        width = entityWidth(player);

        if ((player->position.x + width) > SCREEN_WIDTH)
            player->position.x = SCREEN_WIDTH - width;
    }

    if (player->position.y < 25) // HACK no magic constant (height of border)
    {
        player->position.y = 25;
    }
    else
    {
        height = entityHeight(player);

        if ((player->position.y + height) > (SCREEN_HEIGHT + 5))
            player->position.y = (SCREEN_HEIGHT + 5) - height;
    }
}

void playGame(void)
{
    Image background;
    Player* player;
    F16* plane;
    F16* plane2;
    uint8 buffer;
    Behavior think;

    setDoubleBuffer(true);

    background.address = 0x002BAD40;
    background.width = 0xA0;
    background.height = 0xF0;

    player = createPlayer(makePoint(80, 80));
    plane = createF16(makePoint(20, -500), 10, boringF16);
    plane2 = createF16(makePoint(100, -400), 10, boringF16);

    while (player->health > 0)
    {
        drawBackground(&background);
        drawEntity(player);
        drawEntity(plane);
        drawEntity(plane2);

        thinkEnemy(plane);
        thinkEnemy(plane2);

        handlePlayerInput(player);

        scrollBackground(&background);
        flipBuffer(&buffer);
        delay_ms(16);
    }
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
