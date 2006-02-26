#ifndef CLOUD_H
#define CLOUD_H

#include "common.h"
#include "updateable.h"

#define CLOUD_TINY   0
#define CLOUD_SMALL  1
#define CLOUD_MEDIUM 2
#define CLOUD_LARGE  3

typedef struct Cloud
{
    UPDATEABLE_BASE;
    int8 speed;
} Cloud;

Cloud* createCloud(int8 x, uint8 recess, uint8 speed, uint8 size);

#endif // CLOUD_H
