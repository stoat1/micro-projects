#ifndef PARETODISTRIBUTION_H_INCLUDED
#define PARETODISTRIBUTION_H_INCLUDED

#include "Distribution.h"
#include <stdexcept>

class ParetoDistribution : public Distribution
{
public:

    ParetoDistribution( double minX, double alpha )
        throw( std::invalid_argument );
    ParetoDistribution( const ParetoDistribution & orig );
    ParetoDistribution & operator=( const ParetoDistribution & orig );

    virtual ~ParetoDistribution();

    virtual double nextValue();

private:

    double minX;
    double alpha;
    Distribution * uniform;
};


#endif // PARETODISTRIBUTION_H_INCLUDED
