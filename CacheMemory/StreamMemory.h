#ifndef STREMARRAY_H_INCLUDED
#define STREMARRAY_H_INCLUDED

#include "AbstractMemory.h"
#include <iostream>

class StreamMemory : public AbstractMemory
{
public:

    typedef AbstractMemory::address_t address_t;
    typedef AbstractMemory::value_t value_t;

    // constructor and destructor
    explicit StreamMemory( std::iostream & );
    virtual ~StreamMemory();

    // read and write
    value_t get( address_t );
    void set( address_t, value_t );
    address_t size() const;

private:

    std::iostream & stream;
    std::streampos streamSize;
};

#endif // STREMARRAY_H_INCLUDED
