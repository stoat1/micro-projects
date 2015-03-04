#include "UniformDistribution.h"

UniformDistribution::UniformDistribution( const Random & random )
: random( random )
{}

UniformDistribution::~UniformDistribution()
{}

double UniformDistribution::nextValue()
{
    return static_cast<double>( random.nextValue() ) / random.maxValue();
}
