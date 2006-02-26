#include "world.h"
#include "updateable.h"
#include "projectile.h"
#include "player.h"
#include "enemy.h"
#include "background.h"
#include "collision.h"
#include "level.h"

static void destroyWorld(World* w)
{
    Node* curr;

    for_each (curr, w->updateables)
        destroy(curr->data);

    for_each (curr, w->enemies)
        destroy(curr->data);

    for_each (curr, w->playerProjectiles)
        destroy(curr->data);

    for_each (curr, w->enemyProjectiles)
        destroy(curr->data);

    for_each (curr, w->overlays)
        destroy(curr->data);

    for_each (curr, w->underlays)
        destroy(curr->data);

    destroy(w->updateables);
    destroy(w->enemies);
    destroy(w->playerProjectiles);
    destroy(w->enemyProjectiles);
    destroy(w->overlays);
    destroy(w->underlays);

    destroy(w->player);
}

World* createWorld(Player* player, Level* level)
{
    World* w = new(World);
    w->destroy = destroyWorld;

    w->updateables = createList();
    w->enemies = createList();
    w->playerProjectiles = createList();
    w->enemyProjectiles = createList();
    w->overlays = createList();
    w->underlays = createList();

    w->player = player;
    w->level = level;

    w->active = true;

    return w;
}

bool active(World* w)
{
    return w->active;
}

void addUpdateable(World* world, Updateable* u)
{
    appendElement(world->updateables, u);
}

void addEnemy(World* world, Enemy* e)
{
    appendElement(world->enemies, e);
}

void addPlayerProjectile(World* world, Projectile* p)
{
    appendElement(world->playerProjectiles, p);
}

void addEnemyProjectile(World* world, Projectile* p)
{
    appendElement(world->enemyProjectiles, p);
}

void addOverlay(World* world, Updateable* u)
{
    appendElement(world->overlays, u);
}

void addUnderlay(World* world, Updateable* u)
{
    appendElement(world->underlays, u);
}

Node* removeUpdateable(World* world, Node* u)
{
    destroy(u->data);
    return removeNode(world->updateables, u);
}

Node* removeEnemy(World* world, Node* e)
{
    destroy(e->data);
    return removeNode(world->enemies, e);
}

Node* removePlayerProjectile(World* world, Node* p)
{
    destroy(p->data);
    return removeNode(world->playerProjectiles, p);
}

Node* removeEnemyProjectile(World* world, Node* p)
{
    destroy(p->data);
    return removeNode(world->enemyProjectiles, p);
}

Node* removeOverlay(World* world, Node* u)
{
    destroy(u->data);
    return removeNode(world->overlays, u);
}

Node* removeUnderlay(World* world, Node* u)
{
    destroy(u->data);
    return removeNode(world->underlays, u);
}

void drawWorld(World* world)
{
    Node* curr;

    drawEntity(world->level->background);

    for_each (curr, world->underlays)
        drawEntity(curr->data);

    for_each (curr, world->enemies)
        drawEntity(curr->data);

    drawEntity(world->player);

    for_each (curr, world->playerProjectiles)
        drawEntity(curr->data);

    for_each (curr, world->enemyProjectiles)
        drawEntity(curr->data);

    for_each (curr, world->updateables)
        drawEntity(curr->data);

    for_each (curr, world->overlays)
        drawEntity(curr->data);
}

void updateWorld(World* world)
{
    Node* curr;

    update(world->player, world);
    update(world->level->background, world);
    updateLevel(world->level, world);
    
    curr = world->updateables->head;
    while (curr != NULL)
    {
        if (update(curr->data, world) == UPDATE_REMOVE)
            curr = removeUpdateable(world, curr);
        else
            curr = curr->next;
    }

    curr = world->enemies->head;
    while (curr != NULL)
    {
        if (update(curr->data, world) == UPDATE_REMOVE)
            curr = removeEnemy(world, curr);
        else
            curr = curr->next;
    }

    curr = world->playerProjectiles->head;
    while (curr != NULL)
    {
        if (update(curr->data, world) == UPDATE_REMOVE)
            curr = removePlayerProjectile(world, curr);
        else
            curr = curr->next;
    }

    curr = world->enemyProjectiles->head;
    while (curr != NULL)
    {
        if (update(curr->data, world) == UPDATE_REMOVE)
            curr = removeEnemyProjectile(world, curr);
        else
            curr = curr->next;
    }

    curr = world->overlays->head;
    while (curr != NULL)
    {
        if (update(curr->data, world) == UPDATE_REMOVE)
            curr = removeOverlay(world, curr);
        else
            curr = curr->next;
    }

    curr = world->underlays->head;
    while (curr != NULL)
    {
        if (update(curr->data, world) == UPDATE_REMOVE)
            curr = removeUnderlay(world, curr);
        else
            curr = curr->next;
    }
}

void collideWorld(World* world)
{
    Node* proj;
    Node* enemy;
    bool killed;
    
    enemy = world->enemies->head;
    while (enemy != NULL)
    {
        killed = false;

        proj = collides(enemy->data, world->playerProjectiles);
        if (proj != NULL)
        {
            impact(proj->data, enemy->data, world);
            if (damage(enemy->data, ((Projectile*)proj->data)->damage))
            {
                killed = true;
                kill(enemy->data, world);
            }

            removePlayerProjectile(world, proj);
        }

        if (killed)
            enemy = removeEnemy(world, enemy);
        else
            enemy = enemy->next;
    }

    proj = collides(world->player, world->enemyProjectiles);
    if (proj != NULL)
    {
        impact(proj->data, player, world);
        if (damage(player, ((Projectile*)proj->data)->damage))
            kill(player, world);

        // world->active = false;
    }
}
