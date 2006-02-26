#include "destroyable.h"

void nullDestroy(Destroyable* what)
{
    // nothing to see here
}

void destroy(Destroyable* what)
{
    what->destroy(what);
    free(what);
}
