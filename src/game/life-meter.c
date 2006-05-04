#include "life-meter.h"
#include "life.anim.inl"

LifeMeter* createLifeMeter(Player* who, Point where)
{
    LifeMeter* m = new(LifeMeter);
    m->destroy = nullDestroy;
    m->animation = lifeAnimation();
    m->position = where;
    m->player = who;
    m->currentFrame = 0;

    return m;
}

void drawLifeMeter(LifeMeter* n)
{
    Point where = n->position;
    uint8 lives = n->player->lives;
    uint8 i;

    if (lives > 0)
    {
        for (i = 0 ; i < lives - 1; ++i)
        {
            drawImage(&n->animation->images[0], where, true);
            where.x -= 7;
        }
    }
}

