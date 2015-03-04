#include "ParetoDistribution.h"
#include "UniformDistribution.h"
#include <cmath>

using namespace std;

ParetoDistribution::ParetoDistribution( double minX, double alpha )
    throw( invalid_argument )
: minX( minX )
, alpha( alpha )
, uniform( NULL )
{
    if( minX <= 0 )
    {
        throw invalid_argument( "ParetoDistribution: minX must be positive" );
    }

    if( alpha <= 0 )
    {
        throw invalid_argument( "ParetoDistribution: alpha must be positive" );
    }

    uniform = new UniformDistribution( Random( 0 ) );
}

ParetoDistribution::~ParetoDistribution()
{
    delete uniform;
}

double ParetoDistribution::nextValue()
{
    double u;
    do
    {
        u = uniform->nextValue();
    }
    while( u <= 0 || 1 <= u );
    return minX / pow( u, 1 / alpha );
}
