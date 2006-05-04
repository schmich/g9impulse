#include "player.anim.inl"
#include "weapon-indicator.h"

void drawWeaponIndicator(WeaponIndicator* w)
{
    Point where = w->position;
    uint8 i;
    uint8 index;

    for (i = 0; i < 3; ++i)
    {
        if (i == w->player->weaponClass)
            index = i + 3;
        else
            index = i;

        drawImage(&w->animation->images[index], where, true);
        where.x += 5;
    }
}

WeaponIndicator* createWeaponIndicator(Player* who, Point where)
{
    WeaponIndicator* w = new(WeaponIndicator);
    w->destroy = nullDestroy;
    w->animation = weaponIconAnimation();
    w->position = where;
    w->player = who;
    w->currentFrame = 0;

    return w;
}
