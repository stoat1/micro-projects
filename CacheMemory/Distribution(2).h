#ifndef DISTRIBUTION_H_INCLUDED
#define DISTRIBUTION_H_INCLUDED

class Distribution
{
public:

    virtual ~Distribution() {}

    virtual double nextValue() = 0;
};

#endif // DISTRIBUTION_H_INCLUDED
