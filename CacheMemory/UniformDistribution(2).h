#ifndef UNIFORMDISTRIBUTION_H_INCLUDED
#define UNIFORMDISTRIBUTION_H_INCLUDED

#include "Distribution.h"
#include "Random.h"

class UniformDistribution : public Distribution
{
public:

    explicit UniformDistribution( const Random & random );
    virtual ~UniformDistribution();

    virtual double nextValue();

private:

    Random random;
};

#endif // UNIFORMDISTRIBUTION_H_INCLUDED
