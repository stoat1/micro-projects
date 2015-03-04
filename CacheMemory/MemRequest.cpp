#include "MemRequest.h"

using namespace std;

MemRequest::MemRequest(
    address_t begin,
    address_t end,
    Operation op,
    unsigned int seed
) throw( std::invalid_argument )
: seed( seed ), begin( begin ), end( end ), op( op )
{
    if( end < begin )
    {
        throw std::invalid_argument(
                        "MemRequest: end index must be >= begin index" );
    }
}

void MemRequest::execute( AbstractMemory & storage )
{
    Random random( seed );

    if( WRITING == op )
    {
        for( address_t addr = begin; addr < end; ++addr )
        {
            value_t value = 'a' + random.nextValue() % 64;
            storage.set( addr, value );
        }
    }
    else /* READING == op */
    {
        for( address_t addr = begin; addr < end; ++addr )
        {
            storage.get( addr );
        }
    }
}

void MemRequest::toVector( vector<address_t> & v )
{
    v.clear();
    for( address_t addr = begin; addr < end; ++addr )
    {
        v.push_back( addr );
    }
}

size_t MemRequest::size() const
{
    return end - begin;
}
