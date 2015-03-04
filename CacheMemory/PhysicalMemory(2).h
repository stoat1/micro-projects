#ifndef PHYSICALARRAY_H_INCLUDED
#define PHYSICALARRAY_H_INCLUDED

#include "AbstractMemory.h"

#include <vector>

class PhysicalMemory : public AbstractMemory
{
public:

    typedef AbstractMemory::address_t address_t;
    typedef AbstractMemory::value_t value_t;

    // constructor and destructor
    explicit PhysicalMemory( address_t size );
    ~PhysicalMemory();

    // read and write
    value_t get( address_t ) throw( std::range_error );
    void set( address_t, value_t ) throw( std::range_error );

    // elems in array
    address_t size() const;

private:

    std::vector<value_t> vect;
};

#endif // PHYSICALARRAY_H_INCLUDED
