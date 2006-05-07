#include "awards.h"
#include "input.h"
#include "text.h"

static void destroyAwardsOverlay(AwardsOverlay* a)
{
    destroy(a->awards);
}

Awards* createAwards(void)
{
    Awards* a = new(Awards);
    a->destroy = nullDestroy;

    a->sharpShooter     =
    a->survivor         =
    a->packrat          =
    a->greenpeace       =
    a->spammer          =
    a->bulletMagnet     =
    a->punisher         =
    a->pacifist         =
    a->masochist        = false;

    a->sharpShooterBonus    =
    a->survivorBonus        =
    a->packratBonus         =
    a->greenpeaceBonus      =
    a->spammerBonus         =
    a->bulletMagnetBonus    =
    a->punisherBonus        =
    a->pacifistBonus        =
    a->masochistBonus       = 0;

    return a;
}

#define ADD_BONUS(x) if (a->x) { bonus += a->x##Bonus; }

uint16 totalBonus(Awards* a)
{
    uint16 bonus = 0;

    ADD_BONUS(sharpShooter);
    ADD_BONUS(survivor);
    ADD_BONUS(packrat);
    ADD_BONUS(greenpeace);
    ADD_BONUS(spammer);
    ADD_BONUS(bulletMagnet);
    ADD_BONUS(punisher);
    ADD_BONUS(pacifist);
    ADD_BONUS(masochist);

    return bonus;
}

#define COUNT_BONUS(x) if (a->x) { ++count; }

uint8 numAwards(Awards* a)
{
    uint8 count = 0;

    COUNT_BONUS(sharpShooter);
    COUNT_BONUS(survivor);
    COUNT_BONUS(packrat);
    COUNT_BONUS(greenpeace);
    COUNT_BONUS(spammer);
    COUNT_BONUS(bulletMagnet);
    COUNT_BONUS(punisher);
    COUNT_BONUS(pacifist);
    COUNT_BONUS(masochist);

    return count;
}

#define SET_BONUS(x, bonus) { a->awards->x = true; a->awards->x##Bonus = bonus; }

AwardsOverlay* createAwardsOverlay(Player* who)
{
    AwardsOverlay* a = new(AwardsOverlay);
    Stats* s = who->stats;

    uint16 shotAcc;
    uint16 artAcc;
    uint16 killPct;

    a->destroy = destroyAwardsOverlay;
    a->awards = createAwards();
    a->showStep = 0;
    a->awardsShown = 0;
    a->player = who;

    //
    // calculate player awards
    //

    if (s->shotsFired > 0)
    {
        shotAcc = (s->shotsLanded * 100) / s->shotsFired;
        if (shotAcc > 50)
            SET_BONUS(sharpShooter, 10 * (shotAcc - 50) + 250);
    }

    if (s->healthLost < 6)
        SET_BONUS(survivor, (6 - s->healthLost) * 100);

    if (s->enemiesFaced > 0)
    {
        killPct = (s->kills * 100) / s->enemiesFaced;
        if (killPct > 75)
            SET_BONUS(punisher, 25 * (killPct - 75) + 500);
    }

    if (s->artifactsGiven > 0)
    {
        artAcc = (s->artifactsCollected * 100) / s->artifactsGiven;
        if (artAcc > 75)
            SET_BONUS(packrat, 5 * (artAcc - 75) + 100);
    }

    if (s->nukesFired == 0)
        SET_BONUS(greenpeace, 1000);

    if (s->kamikazes >= 5)
        SET_BONUS(masochist, 750);

    if (s->healthLost > 20)
        SET_BONUS(bulletMagnet, 1000);

    a->numAwards = numAwards(a->awards);
    a->finalScore = who->stats->score + totalBonus(a->awards);
    a->scoreStep = 0;
    a->screenTimeout = 0;

    return a;
}

bool updateAwardsOverlay(AwardsOverlay* a)
{
    Input* input = getInputEvent();
    uint16 step = a->finalScore / 100;

    if (a->awardsShown <= a->numAwards)
    {
        if (++a->showStep == 50)
        {
            a->showStep = 0;
            ++a->awardsShown;
        }
    }
    else if (a->scoreStep < a->finalScore)
    {
        if (a->scoreStep + step <= a->finalScore)
            a->scoreStep += step;
        else
            a->scoreStep = a->finalScore;
    }

    if (a->scoreStep == a->finalScore)
    {
        if (input->startPressed)
            return true;

        if (++a->screenTimeout == 300)
            return true;
    }

    return false;
}

void drawAward(const char* desc, uint16 bonus, Point where)
{
    static char plus[] = "+";

    drawText(desc, where, COLOR_WHITE);

    where.x += 110;
    drawText(plus, where, COLOR_WHITE);

    where.x += textWidth(plus);
    drawNumber(bonus, where, COLOR_WHITE);
}

#define DRAW_AWARD(x) if (a->x && count++ < ao->awardsShown) { \
    drawAward(x##Text, a->x##Bonus, where); where.y += 20; \
}

void drawAwardsOverlay(AwardsOverlay* ao)
{
    Awards* a = ao->awards;
    Point where = makePoint(5, 60);
    uint8 count = 0;

    static char gameOver[] = "GAME OVER";
    static char finalScore[] = "FINAL SCORE";
    static char pressStart[] = "PRESS START";
    static char noAwards[] = "NO AWARDS";

    static char sharpShooterText[]  = "SHARP SHOOTER";
    static char survivorText[]      = "SURVIVOR";
    static char packratText[]       = "PACKRAT";
    static char greenpeaceText[]    = "GREENPEACE";
    static char spammerText[]       = "SPAMMER";
    static char bulletMagnetText[]  = "BULLET MAGNET";
    static char punisherText[]      = "PUNISHER";
    static char pacifistText[]      = "PACIFIST";
    static char masochistText[]     = "MASOCHIST";

    drawTextCentered(gameOver, makePoint(80, 20), COLOR_BLUE);

    if (ao->numAwards == 0)
    {
        drawTextCentered(noAwards, makePoint(80, 60), COLOR_WHITE);
        where.y += 20;
    }

    DRAW_AWARD(sharpShooter);
    DRAW_AWARD(survivor);
    DRAW_AWARD(packrat);
    DRAW_AWARD(greenpeace);
    DRAW_AWARD(spammer);
    DRAW_AWARD(bulletMagnet);
    DRAW_AWARD(punisher);
    DRAW_AWARD(pacifist);
    DRAW_AWARD(masochist);

    if (ao->awardsShown > ao->numAwards)
    {
        where.x = 80;
        where.y += 20;
        drawTextCentered(finalScore, where, COLOR_BLUE);

        where.y += 20;
        drawNumberCentered(ao->scoreStep, where, COLOR_BLUE);
    }

    if (ao->scoreStep == ao->finalScore)
    {
        where.y += 40;
        drawTextCentered(pressStart, where, COLOR_WHITE);
    }
}
