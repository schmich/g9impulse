#ifndef CLOUD_H
#define CLOUD_H

#include "common.h"
#include "updateable.h"
#include "fraction.h"

#define CLOUD_TINY   0
#define CLOUD_SMALL  1
#define CLOUD_MEDIUM 2
#define CLOUD_LARGE  3

typedef struct Cloud
{
    UPDATEABLE_BASE;
} Cloud;

Cloud* createCloud(int16 x, int16 recess, Fraction speed, uint8 size);

#endif // CLOUD_H
