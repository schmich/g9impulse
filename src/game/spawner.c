#include "spawner.h"
#include "spawner.anim.inl"

static void destroySpawner(Spawner* s)
{
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
                addEnemy(w, s->spawnee);
                break;

            case SPAWNER_ENEMY_PROJECTILE:
                addEnemyProjectile(w, s->spawnee);
                break;

            case SPAWNER_PLAYER_PROJECTILE:
                addPlayerProjectile(w, s->spawnee);
                break;

            case SPAWNER_ARTIFACT:
                addArtifact(w, s->spawnee);
                break;
        }

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

    return s;
}
