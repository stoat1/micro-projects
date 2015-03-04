#include "IntervalRequest.h"

IntervalRequest::IntervalRequest( Interval anInterval, Operation anOperation )
: interval( anInterval ), operation( anOperation ) {}

IntervalRequest::~IntervalRequest()
{}

void IntervalRequest::execute( AbstractMemory & storage )
{
    typedef Interval::address_t address_t;

    switch( operation )
    {
    case READ_OPERATION:
        for( address_t add = interval.begin; add < interval.end; ++add )
        {
            storage.get( add );
        }
        break;

    case WRITE_OPERATION:
        for( address_t add = interval.begin; add < interval.end; ++add )
        {
            storage.set( add, 0 );
        }
        break;
    }
}
