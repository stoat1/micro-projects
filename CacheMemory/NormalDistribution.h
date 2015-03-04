#ifndef NORMALDISTRIBUTION_H_INCLUDED
#define NORMALDISTRIBUTION_H_INCLUDED

#include "Distribution.h"

class NormalDistribution : public Distribution
{
public:

    NormalDistribution( double expectedValue, double dispersion,
                        const Random & random );
    virtual ~NormalDistribution();

    virtual double nextValue();

private:

    UniformDistribution uniform;
    double expectedValue, standardDeviation;
};

#endif // NORMALDISTRIBUTION_H_INCLUDED
