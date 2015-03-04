#ifndef MEMREQUEST_H_INCLUDED
#define MEMREQUEST_H_INCLUDED

#include <stdexcept>
#include <vector>

#include "AbstractMemory.h"
#include "Random.h"

class MemRequest
{
public:

    typedef AbstractMemory::address_t address_t;
    typedef AbstractMemory::value_t value_t;
    enum Operation { WRITING, READING };

    // constructor
    // begin, end - interval of requests
    // op - kind of request (write or read)
    // seed - seed for generating values for writing
    MemRequest(
        address_t begin,
        address_t end,
        Operation op,
        unsigned int seed = 0
    ) throw( std::invalid_argument );

    // invoke request on a given storage
    void execute( AbstractMemory & storage );

    // save request address sequence in vector
    void toVector( std::vector<address_t> & v );

    // return count of references execute() invokes
    size_t size() const;

private:

    unsigned int seed;
    address_t begin, end;
    Operation op;
};

#endif // MEMREQUEST_H_INCLUDED
