#include "stats.h"

Stats* createStats()
{
    Stats* s = new(Stats);

    s->destroy = nullDestroy;

    s->nukesFired           =
    s->shotsFired           =
    s->shotsLanded          =
    s->artifactsGiven       =
    s->artifactsCollected   =
    s->enemiesFaced         =
    s->shotsFaced           =
    s->kills                =
    s->score                =
    s->livesLost            =
    s->healthLost           =
    s->kamikazes            = 0;

    return s;
}
