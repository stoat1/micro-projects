#include "Random.h"
#include <climits>
#include <cmath>

Random::Random( unsigned int seed )
: A( 1664525 ), C( 1013904223 ), x( seed ), MAX_VALUE( UINT_MAX )
{}

void Random::reset( unsigned int seed )
{
    x = seed;
}

unsigned int Random::nextValue()
{
    return x = A * x + C;
}

unsigned int Random::maxValue() const
{
    return MAX_VALUE;
}
