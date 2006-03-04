#include "collision.h"

bool intersects(Sprite* p, Sprite* q)
{
    int16 pl, pr, pt, pb;
    int16 ql, qr, qt, qb;

    qt = spriteTop(q);
    pb = spriteBottom(p);
    if (qt > pb)
        return false;

    qb = spriteBottom(q);
    pt = spriteTop(p);
    if (qb < pt)
        return false;

    qr = spriteRight(q);
    pl = spriteLeft(p);
    if (qr < pl)
        return false;

    ql = spriteLeft(q);
    pr = spriteRight(p);
    if (ql > pr)
        return false;

    return true;
}

Node* collides(Sprite* who, List* items)
{
    Node* item;

    for (item = items->head; item != NULL; item = item->next)
    {
        if (intersects(item->data, who))
            return item;
    }

    return NULL;
}
