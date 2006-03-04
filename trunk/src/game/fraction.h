#ifndef FRACTION_H
#define FRACTION_H

#include "common.h"

typedef struct Fraction
{
    int8 numerator;
    int8 denominator;
} Fraction;

Fraction makeFraction(int8 numerator, int8 denominator);
Fraction makeWhole(int8 value);

#endif // FRACTION_H
