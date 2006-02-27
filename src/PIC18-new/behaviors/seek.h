#ifndef SEEK_H
#define SEEK_H

#include "common.h"
#include "updateable.h"

typedef struct Seek
{
    BEHAVIOR_BASE;
    bool locked;
    int16 horizSpeed;
} Seek;

Seek* createSeek(void);

#endif // SEEK_H
