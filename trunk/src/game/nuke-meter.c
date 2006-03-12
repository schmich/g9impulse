#include "nuke-meter.h"
#include "nuke.anim.inl"

NukeMeter* createNukeMeter(Player* who, Point where)
{
    NukeMeter* n = new(NukeMeter);
    n->destroy = nullDestroy;
    n->animation = nukeAnimation();
    n->position = where;
    n->player = who;
    n->currentFrame = 0;

    return n;
}

void drawNukeMeter(NukeMeter* n)
{
    Point where = n->position;

    uint8 i;
    for (i = 0 ; i < n->player->nukes; ++i)
    {
        drawImage(&n->animation->images[0], where, true);
        where.x -= 5;
    }
}
