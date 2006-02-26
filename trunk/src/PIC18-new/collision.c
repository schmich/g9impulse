#include "collision.h"

bool intersects(Entity* p, Entity* q)
{
    int16 pl, pr, pt, pb;
    int16 ql, qr, qt, qb;

    qt = entityTop(q);
    pb = entityBottom(p);
    if (qt > pb)
        return false;

    qb = entityBottom(q);
    pt = entityTop(p);
    if (qb < pt)
        return false;

    qr = entityRight(q);
    pl = entityLeft(p);
    if (qr < pl)
        return false;

    ql = entityLeft(q);
    pr = entityRight(p);
    if (ql > pr)
        return false;

    return true;
}

Node* collides(Entity* who, List* items)
{
    Node* item;

    for (item = items->head; item != NULL; item = item->next)
    {
        if (intersects(item->data, who))
            return item;
    }

    return NULL;
}
