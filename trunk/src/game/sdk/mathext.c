#include "mathext.h"
#include <stdlib.h>

int16 random(int16 lo, int16 hi)
{
    return (rand() % (hi - lo + 1)) + lo;
}

int16 abs(int16 x)
{
    if (x < 0)
        return -x;
    else
        return x;
}
