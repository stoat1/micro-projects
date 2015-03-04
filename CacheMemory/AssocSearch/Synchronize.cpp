#include "Synchronize.h"
#include <string>

using namespace std;

Synchronize::Synchronize( size_t count ) throw( RuntimeError, InvalidArgument )
: totalCount( count ), insideCount( 0 ),
#ifdef TWO_EVENTS_IMPLEMENTATION
pAutoEvent1( NULL ), pAutoEvent2( NULL )
#else
pMux1( NULL ), pMux2( NULL ),
pEvent1( NULL ), pEvent2( NULL )
#endif
{
    // check argument
    if( 0 == count )
    {
        throw InvalidArgument( "Count must be a positive integer" );
    }

    // create mutexes and events
    try
    {
#       ifdef TWO_EVENTS_IMPLEMENTATION
        pAutoEvent1.reset( new AutoResetEvent( true ) );
        pAutoEvent2.reset( new AutoResetEvent( false ) );
#       else
        pMux1.reset( new Mutex() );
        pMux2.reset( new Mutex() );

        pEvent1.reset( new ManualResetEvent( true ) );
        pEvent2.reset( new ManualResetEvent( false ) );
#       endif
    }
    catch( ApiError & ex )
    {
        throw RuntimeError( string( "cannot create mutex or event: " ) + ex.what() );
    }
}

Synchronize::~Synchronize()
{}

// syncronization procedure
void Synchronize::operator()() throw( RuntimeError )
{
    try
    {
#       ifdef TWO_EVENTS_IMPLEMENTATION
        pAutoEvent1->waitAndReset();
        ++insideCount;
        if( totalCount == insideCount )
        {
            pAutoEvent2->setSignaled( true );
        }
        else
        {
            pAutoEvent1->setSignaled( true );
        }

        pAutoEvent2->waitAndReset();
        --insideCount;
        if( 0 == insideCount )
        {
            pAutoEvent1->setSignaled( true );
        }
        else
        {
            pAutoEvent2->setSignaled( true );
        }
#       else
        pEvent1->wait();

        pMux1->waitAndOwn();
        {
            // critical section 1

            ++insideCount;
            if( totalCount == insideCount )
            {
                // I 'm last entrancing

                // thr order is matter!
                pEvent1->setSignaled( false );
                pEvent2->setSignaled( true );
            }
        }
        pMux1->release();

        pEvent2->wait();

        pMux2->waitAndOwn();
        {
            // critical section 2

            -- insideCount;
            if( 0 == insideCount )
            {
                // I 'm last going out

                // thr order is matter!
                pEvent2->setSignaled( false );
                pEvent1->setSignaled( true );
            }
        }
        pMux2->release();
#       endif
    }
    catch( ApiError & ex )
    {
        throw RuntimeError( ex.what() );
    }
}

