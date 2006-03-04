#include "world.h"
#include "updateable.h"
#include "projectile.h"
#include "player.h"
#include "enemy.h"
#include "background.h"
#include "collision.h"
#include "level.h"
#include "artifact.h"
#include "health.anim.inl"

Sprite* theHealthSprite = NULL;

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

    for_each (curr, w->artifacts)
        destroy(curr->data);

    destroy(w->updateables);
    destroy(w->enemies);
    destroy(w->playerProjectiles);
    destroy(w->enemyProjectiles);
    destroy(w->overlays);
    destroy(w->underlays);
    destroy(w->artifacts);

    destroy(w->player);
    destroyStatic(w->level);
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
    w->artifacts = createList();

    w->player = player;
    w->level = level;

    w->active = true;

    if (!theHealthSprite)
    {
        theHealthSprite = new(Sprite);
        theHealthSprite->destroy = nullDestroy;
        theHealthSprite->position = makePoint(130, 230);
        theHealthSprite->animation = healthAnimation();

        animationBeginning(theHealthSprite);
    }

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

    for_each (curr, world->underlays)
        drawSprite(curr->data);

    for_each (curr, world->enemies)
        drawSprite(curr->data);

    if (!dead(world->player))
        drawSprite(world->player);

    theHealthSprite->currentFrame = 6 - world->player->health;
    drawSprite(theHealthSprite);

    for_each (curr, world->playerProjectiles)
        drawSprite(curr->data);

    for_each (curr, world->enemyProjectiles)
        drawSprite(curr->data);

    for_each (curr, world->updateables)
        drawSprite(curr->data);

    for_each (curr, world->overlays)
        drawSprite(curr->data);
}

void updateWorld(World* world)
{
    Node* curr;

    if (!dead(world->player))
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

    if (dead(world->player) && (world->updateables->head == NULL))
        world->active = false;
}

void collideWorld(World* world)
{
    Node* proj;
    Node* enemy;
    Node* artifact;
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

    if (!dead(world->player))
    {
        proj = collides(world->player, world->enemyProjectiles);
        if (proj != NULL)
        {
            impact(proj->data, world->player, world);

            if (damage(world->player, ((Projectile*)proj->data)->damage))
                kill(world->player, world);

            removeEnemyProjectile(world, proj);
        }

        artifact = collides(world->player, world->artifacts);
        if (artifact != NULL)
        {
            affect(artifact->data, world->player, world);
            removeArtifact(world, artifact);
        }

        enemy = collides(world->player, world->enemies);
        if (enemy != NULL)
        {
            if (damage(world->player, 2))
                kill(world->player, world);

            kill(enemy->data, world);
            removeEnemy(world, enemy);
        }
    }
}
