#ifndef PARETOREQUESTS_H_INCLUDED
#define PARETOREQUESTS_H_INCLUDED

#include <deque>
#include <stdexcept>

#include "RequestGenerator.h"
#include "ParetoDistribution.h"

class ParetoRequests : public RequestGenerator
{
public:

    ParetoRequests(
        size_t storageSize,
        size_t dequeSize,
        size_t minGroupSize,
        double alpha
    ) throw( std::invalid_argument );

    virtual ~ParetoRequests();

    virtual MemRequest nextRequest();

private:

    struct File
    {
    public:

        size_t address;
        size_t size;
    };

    void createFile( File & file );

    size_t storageSize;
    ParetoDistribution pareto;
    Random rand;

    std::deque<File> files;
};

#endif // PARETOREQUESTS_H_INCLUDED
