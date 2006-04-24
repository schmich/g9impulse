#include "text.h"
#include "image.h"
#include "text.anim.inl"

static uint8 indexOf(char c)
{
    Animation* font = textAnimation();

    if (c >= 'A' && c <= 'Z')
        return c - 'A' + 10;
    else if (c >= 'a' && c <= 'z')
        return c - 'a' + 10;
    else if (c >= '0' && c <= '9')
        return c - '0';
    else if (c == '_')
        return 36;
    else if (c == ' ')
        return 37; // space
    else
        return 38; // "unknown" character
}

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

void drawText(const char* text, Point where)
{
    Animation* font = textAnimation();

    uint8 index;
    const char* pos;

    for (pos = text; *pos != NULL; ++pos)
    {
        index = indexOf(*pos);

        drawImage(&font->images[index], where, true);
        where.x += font->images[index].width;
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

uint16 textWidth(const char* text)
{
    Animation* font = textAnimation();

    uint16 width = 0;
    const char* pos;

    for (pos = text; *pos != NULL; ++pos)
        width += font->images[indexOf(*pos)].width;

    return width;
}
