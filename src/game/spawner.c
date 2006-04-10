#include "spawner.h"
#include "world.h"
#include "spawner.anim.inl"

static void destroySpawner(Spawner* s)
{
    if (!s->spawned)
        destroy(s->spawnee);

    destroy(s->behavior);
}

static uint8 updateSpawner(Spawner* s, World* w)
{
    if (s->delay == 0)
    {
        switch (s->type)
        {
            case SPAWNER_UPDATEABLE:
                addUpdateable(w, s->spawnee);
                break;

            case SPAWNER_ENEMY:
                addEnemy(w, (Enemy*)s->spawnee);
                break;

            case SPAWNER_ENEMY_PROJECTILE:
                addEnemyProjectile(w, (Projectile*)s->spawnee);
                break;

            case SPAWNER_PLAYER_PROJECTILE:
                addPlayerProjectile(w, (Projectile*)s->spawnee);
                break;

            case SPAWNER_ARTIFACT:
                addArtifact(w, (Artifact*)s->spawnee);
                break;
        }

        s->spawned = true;

        return UPDATE_REMOVE;
    }
    else
        --s->delay;

    return UPDATE_KEEP;
}

Spawner* createSpawner(Updateable* who, uint8 delay, uint8 type)
{
    Spawner* s = new(Spawner);
    s->destroy = destroySpawner;
    s->position = who->position;
    s->animation = spawnerAnimation();
    s->currentFrame = 0;
    s->behavior = createBehavior(updateSpawner);
    s->spawnee = who;
    s->delay = delay;
    s->type = type;
    s->spawned = false;

    return s;
}
