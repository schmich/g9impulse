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
void updateScore(Player* who, int16 change);
void respawnPlayer(Player* who);

void upgradeWeapon(Player* who);
void rollLeft(Player* who);
void rollRight(Player* who);
void noRoll(Player* who);

#endif // PLAYER_H
