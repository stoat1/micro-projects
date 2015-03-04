#ifndef UNIFORMREQUESTS_H_INCLUDED
#define UNIFORMREQUESTS_H_INCLUDED

#include "RequestGenerator.h"
#include "Random.h"
#include <stdexcept>

class UniformRequests : public RequestGenerator
{
public:

    UniformRequests( size_t storageSize, unsigned int seed = 0 ) throw( std::invalid_argument );
    virtual ~UniformRequests();

    virtual MemRequest nextRequest();

private:

    size_t storageSize;
    Random random;
};

#endif // UNIFORMREQUESTS_H_INCLUDED
