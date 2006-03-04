#include "level.h"

void updateLevel(Level* level, World* world)
{
    uint16 progress = level->background->progress;
    uint16 time = level->spawnTimes[level->currentEvent];

    if ((time != 0) && (time < progress))
    {
        level->spawn(world, time);
        ++level->currentEvent;
    }
}
