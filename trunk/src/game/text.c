#include "text.h"
#include "image.h"
#include "text.anim.inl"

//
// assumes text is only numbers
//
static void drawNumberReverse(uint8* text, Point where)
{
    Animation* font = textAnimation();

    uint8* c;
    for (c = text; *c != NULL; ++c) ;

    for (--c; c >= text; --c)
    {
        drawImage(&font->images[*c - '0'], where, true);
        where.x += font->images[0].width;
    }
}

void drawText(const rom char* text, Point where)
{
    Animation* font = textAnimation();

    const rom char* c;
    for (c = text; *c != NULL; ++c)
    {
        if (*c >= '0' && *c <= '9')
            drawImage(&font->images[*c - '0'], where, true);
        else if (*c >= 'A' && *c <= 'Z')
            drawImage(&font->images[*c - 'A' + 10], where, true);
        else if (*c >= 'a' && *c <= 'z')
            drawImage(&font->images[*c - 'a' + 10], where, true);

        where.x += font->images[0].width;
    }
}

void drawNumber(uint16 num, Point where)
{
    uint16 div10;
    uint16 mod;

    uint8 i = 0;
    Animation* font = textAnimation();

    uint8 buffer[7];
    if (num == 0)
        drawImage(&font->images[0], where, true);
    else
    {
        while (num > 0)
        {
            div10 = num / 10;
            mod = num - (div10 << 3) - (div10 << 1);
            
            buffer[i++] = (uint8)mod + '0';
            num = div10;
        }

        buffer[i] = NULL;
        drawNumberReverse(buffer, where);
    }
}
