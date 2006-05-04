#ifndef AWARDS_H
#define AWARDS_H

#include "common.h"
#include "player.h"

typedef struct Awards
{
    DESTROYABLE_BASE;

    //
    // high shotsLanded/shotsFired ratio
    //
    bool    sharpShooter;
    uint16  sharpShooterBonus;

    //
    // little health lost
    //
    bool    survivor;
    uint16  survivorBonus;

    //
    // many artifacts collected
    //
    bool    packrat;
    uint16  packratBonus;

    //
    // few nukes used
    //
    bool    greenpeace;
    uint16  greenpeaceBonus;

    //
    // many shots fired
    //
    bool    spammer;
    uint16  spammerBonus;

    //
    // lots of health lost
    //
    bool    bulletMagnet;
    uint16  bulletMagnetBonus;

    //
    // many kills
    //
    bool    punisher;
    uint16  punisherBonus;

    //
    // few kills
    //
    bool    pacifist;
    uint16  pacifistBonus;

    //
    // many kamikazes
    //
    bool    masochist;
    uint16  masochistBonus;

} Awards;

typedef struct AwardsOverlay
{
    UPDATEABLE_BASE;

    Player* player;

    Awards* awards;
    uint8   showStep;
    uint8   awardsShown; 

    uint8   numAwards;
    uint16  finalScore;
    uint16  scoreStep;
} AwardsOverlay;

AwardsOverlay* createAwardsOverlay(Player* who);
bool updateAwardsOverlay(AwardsOverlay* a);
void drawAwardsOverlay(AwardsOverlay* a);
uint8  numAwards(Awards* awards);
uint16 totalBonus(Awards* awards);

#endif // AWARDS_H
