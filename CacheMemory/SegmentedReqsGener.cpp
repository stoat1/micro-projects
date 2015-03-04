#include "SegmentedReqsGener.h"
#include <cassert>

using namespace std;

SegmentedReqsGener::SegmentedReqsGener( size_t storageSize,
    Distribution * segSizes, Distribution * addresses,
    Random random ) throw( invalid_argument )
: storageSize( storageSize ), segments(),
    segSizes( segSizes ), addresses( addresses ),
    random( random )
{
    size_t totalSegmentSize = 0,
        newSegSize;

    if( storageSize <= 0 )
    {
        throw invalid_argument( "storageSize must be a positive integer" );
    }

    // create segments
    newSegSize = generateSegmentSize();
    while( totalSegmentSize + newSegSize <= storageSize )
    {
        assert( 0 < newSegSize );

        segments.push_back( newSegSize );
        totalSegmentSize += newSegSize;

        newSegSize = generateSegmentSize();
    }

    // add last segment
    if( totalSegmentSize < storageSize )
    {
        newSegSize = totalSegmentSize - storageSize;
        segments.push_back( newSegSize );
    }
}

SegmentedReqsGener::~SegmentedReqsGener()
{}

MemRequest SegmentedReqsGener::nextRequest()
{
    address_t offset;
    MemRequest::Operation op;

    // generate random offset
    offset = generateOffset();

    // TODO implement segmented requests
    if( random.nextValue() < random.maxValue() / 2 )
    {
        op = MemRequest::WRITING;
    }
    else
    {
        op = MemRequest::READING;
    }
    return MemRequest( offset, offset + 1, op, random.nextValue() );
}

SegmentedReqsGener::address_t SegmentedReqsGener::generateOffset()
{
    double r = addresses->nextValue();
    address_t addr = static_cast<address_t>( r );
//    if( addr < 0 )
//    {
//        addr = 0;
//    }
    if( storageSize <= addr )
    {
        addr = storageSize - 1;
    }

    return addr;
}

size_t SegmentedReqsGener::generateSegmentSize()
{
    return 4;
}
