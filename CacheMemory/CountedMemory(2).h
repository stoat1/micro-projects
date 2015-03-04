#ifndef COUNTEDMEMORY_H_INCLUDED
#define COUNTEDMEMORY_H_INCLUDED

#include "AbstractMemory.h"

class CountedMemory : public AbstractMemory
{
public:

    typedef AbstractMemory::value_t value_t;
    typedef AbstractMemory::address_t address_t;

    typedef size_t count_t;

    CountedMemory( AbstractMemory & mem );

    value_t get( address_t ) throw( std::range_error );
    void set( address_t, value_t ) throw( std::range_error );
    address_t size() const;

    count_t getCount() const;
    void resetCount();

private:
    AbstractMemory & mainMemory;
    count_t referenceCount;
};

#endif // COUNTEDMEMORY_H_INCLUDED
