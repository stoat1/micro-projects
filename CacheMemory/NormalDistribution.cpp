#include "NormalDistribution.h"
#include <stdexcept>
#include <cmath>

using namespace std;

NormalDistribution::NormalDistribution( double expectedValue,
                    double dispersion, const Random & random )
: uniform( random ),
    expectedValue( expectedValue ), standardDeviation( 0.0 )
{
    if( standardDeviation <= 0 )
    {
        throw invalid_argument( "dispersion must be a positive number" );
    }

    standardDeviation = sqrt( dispersion );
}

NormalDistribution::~NormalDistribution()
{}

double NormalDistribution::nextValue()
{
    double r1, r2, stdNormal, result;

    r1 = uniform.nextValue();
    r2 = uniform.nextValue();
    stdNormal = log( -2 * r1 ) * cos( 2 * M_PI * r2 );
    result = expectedValue + standardDeviation * stdNormal;

    return result;
}
