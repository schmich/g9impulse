#include "projectile.h"

void impact(Projectile* proj, Entity* who, World* world)
{
    proj->impact(proj, who, world); 
}
