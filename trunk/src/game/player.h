#ifndef PLAYER_H
#define PLAYER_H

#include "common.h"
#include "entity.h"
#include "projectile.h"
#include "engine.h"
#include "enemy.h"
#include "stats.h"

typedef struct Player
{
    ENTITY_BASE;

    Stats*  stats;

    Engine* engine;
    Point   momentum;

    uint8   weaponClass;
    uint8   weaponLevel[3];

    uint8   lives;

    uint8   nukes;
    uint8   nukeCooldown;

    uint8   heat;
    uint8   heatup;
    uint8   cooldown;
    uint8   maxCooldown;
} Player;

Player* createPlayer(Point where);

void drawPlayer(Player* who);

void enemyKilled(Player* who, Enemy* enemy, bool collided);
bool damagePlayer(Player* who, uint8 amount);
void updateScore(Player* who, int16 change);
void respawnPlayer(Player* who);

void upgradeWeapon(Player* who);
void rollLeft(Player* who);
void rollRight(Player* who);
void noRoll(Player* who);

#endif // PLAYER_H
