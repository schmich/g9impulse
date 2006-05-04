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

AwardsOverlay* createAwardsOverlay(Player* who)
{
    AwardsOverlay* a = new(AwardsOverlay);
    a->destroy = destroyAwardsOverlay;
    a->awards = createAwards();
    a->showStep = 0;
    a->awardsShown = 0;
    a->player = who;

    //
    // calculate player awards
    //
    /*a->awards->sharpShooter = true;
    a->awards->sharpShooterBonus = 100;

    a->awards->masochist = true;
    a->awards->masochistBonus = 200;

    a->awards->bulletMagnet = true;
    a->awards->bulletMagnetBonus = 300;*/

    a->numAwards = numAwards(a->awards);
    a->finalScore = who->stats->score + totalBonus(a->awards);
    a->scoreStep = 0;

    return a;
}

bool updateAwardsOverlay(AwardsOverlay* a)
{
    Input* input = getInputEvent();

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
        if (a->scoreStep + 7 <= a->finalScore)
            a->scoreStep += 7;
        else
            a->scoreStep = a->finalScore;
    }

    if (input->startPressed && a->scoreStep == a->finalScore)
        return true;

    return false;
}

void drawAward(const char* desc, uint16 bonus, Point where)
{
    static char plus[] = "+";

    drawText(desc, where, COLOR_WHITE);

    where.x += 120;
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
}
