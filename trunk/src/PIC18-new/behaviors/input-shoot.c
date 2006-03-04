#include "input-shoot.h"
#include "entity.h"
#include "input.h"

static uint8 updateInputShoot(Entity* who, World* world)
{
    if (getInputEvent()->buttonBPressed)
        fire(who, world);
    
    return UPDATE_KEEP;
}

InputShoot* createInputShoot(void)
{
    InputShoot* is = new(InputShoot);
    is->destroy = nullDestroy;
    is->update = updateInputShoot;

    return is;
}
