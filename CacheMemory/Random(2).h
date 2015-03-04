#ifndef RANDOM_H_INCLUDED
#define RANDOM_H_INCLUDED

#include <stdexcept>

class Random
{
public:

    explicit Random( unsigned int seed );
//    Random( const Random & orig );
//    Random & operator=( const Random & orig );

    void reset( unsigned int seed );

    unsigned int nextValue();
    unsigned int maxValue() const;

    unsigned int nextGeom( double q ) throw( std::invalid_argument );
    bool nextBoolean( double p = 0.5 );

private:

    const int A, C;
    unsigned int x, MAX_VALUE;
};

#endif // RANDOM_H_INCLUDED
