#include "world.h"
#include "updateable.h"
#include "projectile.h"
#include "player.h"

static void destroyWorld(World* w)
{
    Node* curr;
    for (curr = w->updateables->head; curr != NULL; curr = curr->next)
        destroy(curr->data);

    destroy(w->updateables);
    destroy(w->projectiles);
}

World* createWorld(Player* player)
{
    World* w = new(World);
    w->destroy = destroyWorld;
    w->updateables = createList();
    w->projectiles = createList();
    w->player = player;

    return w;
}

void addUpdateable(World* world, Updateable* u)
{
    appendElement(world->updateables, u);
}

void addProjectile(World* world, Projectile* p)
{
    appendElement(world->projectiles, p);
    addUpdateable(world, p);
}
