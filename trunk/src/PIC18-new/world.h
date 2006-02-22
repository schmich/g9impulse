#ifndef WORLD_H
#define WORLD_H

#include "common.h"
#include "destroyable.h"
#include "list.h"

typedef struct Player Player;
typedef struct Updateable Updateable;
typedef struct Projectile Projectile;

typedef struct World
{
    DESTROYABLE_BASE;

    List*   updateables;
    List*   projectiles;
    Player* player;
} World;

World* createWorld(Player* player);

void addUpdateable(World* world, Updateable* u);
void addProjectile(World* world, Projectile* p);

#endif // WORLD_H
