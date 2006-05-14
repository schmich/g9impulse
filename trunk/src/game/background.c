#include "background.h"
#include "animation.h"
#include "player.h"
#include "background.anim.inl"
#include "gpu.h"

#define LOOP_END    7229
#define LOOP_START  6824

#define FAST_START  4300
#define FAST_END    6200

static uint8 updateBackground(Background* bg, World* world)
{
    if (bg->progress > FAST_START && bg->progress < FAST_END)
    {
        world->player->boost = true;

        if (++bg->step == 2)
        {
            bg->step = 0;
            ++bg->progress;
            ++bg->offset;
        }

        bg->progress += 2;
        bg->offset += 2;
    }
    else
    {
        world->player->boost = false;

        if (++bg->step == 2)
        {
            ++bg->progress;
            if (++bg->offset == LOOP_END)
                bg->offset = LOOP_START;

            bg->step = 0;
        }
    }

    return UPDATE_KEEP;
}

static void destroyBackground(Background* bg)
{
    destroy(bg->behavior);
}

Background* createBackground(uint16 initOffset)
{
    Background* bg = new(Background);
    bg->destroy = destroyBackground;
    bg->behavior = createBehavior(updateBackground);
    bg->position = makePoint(0, 0);
    bg->progress = bg->offset = initOffset + SCREEN_HEIGHT;
    bg->step = 0;

    bg->animation = backgroundAnimation();
    animationBeginning(bg);

    return bg;
}

void drawBackground(Background* bg)
{
    rom Image* img = currentImage(bg);
    uint32 offset = (uint32)bg->offset * SCREEN_WIDTH;

    drawFullscreen(img->address - offset);
}
