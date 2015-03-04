#include "UniformRequests.h"

using namespace std;

UniformRequests::UniformRequests( size_t storageSize, unsigned int seed )
throw( invalid_argument )
: storageSize( storageSize ), random( seed )
{
    if( storageSize <= 0 )
    {
        throw invalid_argument( "storageSize must be a positive integer" );
    }
}

UniformRequests::~UniformRequests()
{}

MemRequest UniformRequests::nextRequest()
{
    MemRequest::Operation op;

    if( random.nextBoolean() )
    {
        op = MemRequest::WRITING;
    }
    else
    {
        op = MemRequest::READING;
    }

    MemRequest::address_t addr = random.nextValue() % storageSize;

    return MemRequest( addr, addr + 1, op, 0 );
}
