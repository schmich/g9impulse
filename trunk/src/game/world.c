#include "world.h"
#include "updateable.h"
#include "projectile.h"
#include "player.h"
#include "input.h"
#include "enemy.h"
#include "background.h"
#include "collision.h"
#include "level.h"
#include "artifact.h"
#include "health-meter.h"
#include "heat-meter.h"
#include "nuke-meter.h"
#include "text.h"

#define MAX_RESET_TIME 25

static bool softReset(void)
{
    static uint8 resetCounter = MAX_RESET_TIME;
    Input* input = getInputStatus();

    if (input->buttonBPressed &&
        input->buttonAPressed &&
        input->selectPressed)
    {
        if (--resetCounter == 0)
            return true;
    }
    else
    {
        resetCounter = MAX_RESET_TIME;
    }

    return false;
}

static void destroyWorld(World* w)
{
    Node* curr;

    foreach (curr, w->updateables)
        destroy(curr->data);

    foreach (curr, w->enemies)
        destroy(curr->data);

    foreach (curr, w->playerProjectiles)
        destroy(curr->data);

    foreach (curr, w->enemyProjectiles)
        destroy(curr->data);

    foreach (curr, w->overlays)
        destroy(curr->data);

    foreach (curr, w->underlays)
        destroy(curr->data);

    foreach (curr, w->artifacts)
        destroy(curr->data);

    destroy(w->updateables);
    destroy(w->enemies);
    destroy(w->playerProjectiles);
    destroy(w->enemyProjectiles);
    destroy(w->overlays);
    destroy(w->underlays);
    destroy(w->artifacts);

    destroy(w->healthMeter);
    destroy(w->heatMeter);
    destroy(w->nukeMeter);

    destroy(w->player);
    destroyStatic(w->level);
}

World* createWorld(Player* player, Level* level)
{
    Point start;

    World* w = new(World);
    w->destroy = destroyWorld;

    w->updateables = createList();
    w->enemies = createList();
    w->playerProjectiles = createList();
    w->enemyProjectiles = createList();
    w->overlays = createList();
    w->underlays = createList();
    w->artifacts = createList();

    w->level = level;
    w->player = player;

    start = screenCenterBottom();
    start.y += spriteHeight(w->player) / 2;
    setSpriteCenterTop(w->player,  start);

    w->healthMeter = createHealthMeter(w->player, makePoint(130, 230));
    w->heatMeter = createHeatMeter(w->player, makePoint(1, 230));
    w->nukeMeter = createNukeMeter(w->player, makePoint(155, 3));

#ifdef _DEBUG
    w->intro = false;
#else
    w->intro = true;
#endif

    w->introStep = 0;

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

void addArtifact(World* world, Artifact* a)
{
    appendElement(world->artifacts, a);
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

Node* removeArtifact(World* world, Node* a)
{
    destroy(a->data);
    return removeNode(world->artifacts, a);
}

void drawWorld(World* world)
{
    Node* curr;

    drawBackground(world->level->background);

    foreach (curr, world->underlays)
        drawSprite(curr->data);

    foreach (curr, world->artifacts)
        drawSprite(curr->data);

    foreach (curr, world->playerProjectiles)
        drawSprite(curr->data);

    foreach (curr, world->enemyProjectiles)
        drawSprite(curr->data);

    foreach (curr, world->enemies)
        drawSprite(curr->data);

    if (!dead(world->player))
        drawPlayer(world->player);

    foreach (curr, world->updateables)
        drawSprite(curr->data);

    foreach (curr, world->overlays)
        drawSprite(curr->data);

    drawSprite(world->heatMeter);
    drawSprite(world->healthMeter);
    drawNukeMeter(world->nukeMeter);

    drawNumber(world->player->score, makePoint(2, 3));

#ifdef _DEBUG
    drawNumber(world->level->background->progress, makePoint(2, 17));
    drawNumber(memoryFree(), makePoint(2, 31));
#endif
}

void updateWorld(World* world)
{
    Node* curr;
    int16 stop;

    if (softReset())
        world->active = false;

    if (world->intro)
    {
        update(world->player->engine, world); // HACK this sucks

        if (++world->introStep == 2)
        {
            stop = SCREEN_HEIGHT - (int16)spriteHeight(world->player) * 2;

            if (--world->player->position.y == stop)
                world->intro = false;

            world->introStep = 0;
        }
    }
    else
    {
        if (!dead(world->player))
            update(world->player, world);
    }

    update(world->heatMeter, world);
    update(world->healthMeter, world);

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

    curr = world->artifacts->head;
    while (curr != NULL)
    {
        if (update(curr->data, world) == UPDATE_REMOVE)
            curr = removeArtifact(world, curr);
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

    if (dead(world->player) && (world->updateables->head == NULL))
        world->active = false;
}

void collideWorld(World* world)
{
    Node* proj;
    Node* enemy;
    Node* artifact;
    bool killed;
    
    //
    // collide enemies with player's projectiles
    //
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

            if (!((Projectile*)proj->data)->invincible)
                removePlayerProjectile(world, proj);
        }

        if (killed)
        {
            onEnemyKilled(world->player, enemy->data);
            enemy = removeEnemy(world, enemy);
        }
        else
            enemy = enemy->next;
    }

    if (!dead(world->player))
    {
        //
        // collide player with enemies' projectiles
        //
        proj = collides(world->player, world->enemyProjectiles);
        if (proj != NULL)
        {
            impact(proj->data, world->player, world);

#ifndef _DEBUG
            if (damage(world->player, ((Projectile*)proj->data)->damage))
                kill(world->player, world);
#endif

            if (!((Projectile*)proj->data)->invincible)
                removeEnemyProjectile(world, proj);
        }

        //
        // collide player with artifacts
        //
        artifact = collides(world->player, world->artifacts);
        if (artifact != NULL)
        {
            affect(artifact->data, world->player, world);
            removeArtifact(world, artifact);
        }

        //
        // collide player with enemies themselves
        //
        enemy = collides(world->player, world->enemies);
        if (enemy != NULL)
        {
            if (!((Enemy*)enemy->data)->ground)
            {
#ifndef _DEBUG
                if (damage(world->player, 2))
                    kill(world->player, world);
#endif
                onEnemyKilled(world->player, enemy->data);

                kill(enemy->data, world);
                removeEnemy(world, enemy);
            }
        }
    }
}
