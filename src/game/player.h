#ifndef PLAYER_H
#define PLAYER_H

#include "common.h"
#include "entity.h"
#include "projectile.h"
#include "engine.h"
#include "enemy.h"

typedef struct Player
{
    ENTITY_BASE;

    uint16  kills;
    uint16  score;

    Engine* engine;
    Point   momentum;
    uint8   weaponLevel;

    uint8   nukes;

    uint8   heat;
    uint8   cooldown;
    uint8   maxCooldown;

    uint8   nukeCooldown;
} Player;

Player* createPlayer(Point where);

void drawPlayer(Player* who);

void onEnemyKilled(Player* who, Enemy* enemy);
void updateScore(Player* who, int16 change);

void upgradeWeapon(Player* who);
void rollLeft(Player* who);
void rollRight(Player* who);
void noRoll(Player* who);

#endif // PLAYER_H
