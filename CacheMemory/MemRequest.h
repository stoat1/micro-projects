#ifndef MEMREQUEST_H_INCLUDED
#define MEMREQUEST_H_INCLUDED

#include "AbstractMemory.h"

class MemRequest
{
public:

    typedef AbstractMemory::address_t address_t;
    typedef AbstractMemory::value_t value_t;
    enum Operation { WRITING, READING };

    MemRequset( address_t begin, address_t end, Operation op )
        throw( std::invalid_argument )
    : begin( begin ), end( end ), op( op )
    {
        if( end < begin )
        {
            throw std::invalid_argument(
                            "MemRequest: end index must be >= begin index" );
        }
    }

    void execute( AbstractMemory & storage ) const
    {
        if( WRITING == op )
        {
            for( address_t addr = begin; addr < end; ++addr )
            {
                value_t value = generateValue();
                storage.set( addr, value );
            }
        }
        else /* READING == op */
        {
            for( address_t addr = begin; addr < end; ++addr )
            {
                value_t value = storage.get( addr );
            }
        }
    }

    size_t size() const
    {
        return end - begin;
    }

private:

    value_t generateValue()
    {
        static value_t value = 0;
        return ++value;
    }

    address_t begin, end;
    Operation op;
};

#endif // MEMREQUEST_H_INCLUDED
