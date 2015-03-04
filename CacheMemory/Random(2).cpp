#include "Random.h"
#include <climits>
#include <cmath>

using namespace std;

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

unsigned int Random::nextGeom( double q ) throw( invalid_argument )
{
    if( q < 0 || 1 <= q )
    {
        throw invalid_argument( "q must be in [0, 1)" );
    }

    unsigned int result = 0;

    while( nextBoolean( q ) && ( result != maxValue() ) )
    {
        ++result;
    }

    return result;
}

bool Random::nextBoolean( double p )
{
    if( p < 0 || 1 < p )
    {
        throw invalid_argument( "p must be in [0, 1]" );
    }

    return nextValue() < maxValue() * p;
}
