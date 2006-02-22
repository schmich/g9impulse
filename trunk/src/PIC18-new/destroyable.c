#include "destroyable.h"

static Destroyable theNullDestroyable;
static bool theNullDestroyableInit = false;

static void doNothing(Destroyable* what)
{
    // nothing to see here
}

void destroy(Destroyable* what)
{
    what->destroy(what);
    free(what);
}

void destroyStatic(Destroyable* what)
{
    what->destroy(what);
}

Destroyable* nullDestroy
{
    if (!theNullDestroyableInit)
    {
        theNullDestroyable.destroy = doNothing;
        theNullDestroyableInit = true;
    }

    return &theNullDestroyable;
}
