#include "destroyable.h"

void nullDestroy(Destroyable* what)
{
    // nothing to see here
}

void destroy(Destroyable* what)
{
    if (what == NULL)
        return;

    what->destroy(what);
    free(what);
}

void destroyStatic(Destroyable* what)
{
    if (what == NULL)
        return;

    what->destroy(what);
}
