#ifndef CLOUD_H
#define CLOUD_H

#include "common.h"
#include "updateable.h"

#define CLOUD_TINY   0
#define CLOUD_SMALL  1
#define CLOUD_MEDIUM 2
#define CLOUD_LARGE  3
#define CLOUD_COVER  4

typedef struct Cloud
{
    UPDATEABLE_BASE;
} Cloud;

Cloud* createCloud(int16 x, int16 recess, int8 speed, uint8 size);

#endif // CLOUD_H
