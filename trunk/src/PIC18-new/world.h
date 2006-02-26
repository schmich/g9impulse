#ifndef WORLD_H
#define WORLD_H

#include "common.h"
#include "destroyable.h"
#include "list.h"

typedef struct Player Player;
typedef struct Background Background;
typedef struct Updateable Updateable;
typedef struct Projectile Projectile;
typedef struct Enemy Enemy;
typedef struct Level Level;

typedef struct World
{
    DESTROYABLE_BASE;

    bool    active;

    List*   updateables;
    List*   enemies;
    List*   playerProjectiles;
    List*   enemyProjectiles;
    List*   overlays;
    List*   underlays;

    Player* player;
    Level*  level;
} World;

World* createWorld(Player* player, Level* level);

bool active(World* world);

void drawWorld(World* world);
void updateWorld(World* world);
void collideWorld(World* world);

void addUpdateable(World* world, Updateable* u);
void addPlayerProjectile(World* world, Projectile* p);
void addEnemyProjectile(World* world, Projectile* p);
void addEnemy(World* world, Enemy* e);
void addOverlay(World* world, Updateable* u);
void addUnderlay(World* world, Updateable* u);

Node* removeUpdateable(World* world, Node* u);
Node* removePlayerProjectile(World* world, Node* p);
Node* removeEnemyProjectile(World* world, Node* p);
Node* removeEnemy(World* world, Node* e);
Node* removeOverlay(World* world, Node* u);
Node* removeUnderlay(World* world, Node* u);

#endif // WORLD_H
