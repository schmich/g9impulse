#include "fraction.h"

Fraction makeFraction(int8 numerator, int8 denominator)
{
    Fraction f;

    if (numerator < 0)
    {
        if (denominator < 0)
        {
            numerator = -numerator;
            denominator = -denominator;
        }
    }
    else if (denominator < 0)
    {
        numerator = -numerator;
        denominator = -denominator;
    }

    f.numerator = numerator;
    f.denominator = denominator;

    return f;
}

Fraction makeWhole(int8 value)
{
    return makeFraction(value, 1);
}
