#include "ManualResetEvent.h"

ManualResetEvent::ManualResetEvent( bool signaledState ) throw( ApiError )
: handle( NULL )
{
    BOOL bInitialState = ( signaledState ? TRUE : FALSE );

    handle = CreateEvent(
        NULL /* security attributes */,
        TRUE /* manual reset */,
        bInitialState /* initial state is signaled flag */,
        NULL /* name */
    );

    if( NULL == handle )
    {
        throw ApiError( "CreateEvent() returned NULL", GetLastError() );
    }
}

ManualResetEvent::~ManualResetEvent()
{
    BOOL result;

    result = CloseHandle( handle );
    if( FALSE == result )
    {
        // TODO write about this error to log file
    }
}

void ManualResetEvent::wait() throw( ApiError )
{
    DWORD result;

    result = WaitForSingleObject( handle, INFINITE );
    if( result != WAIT_OBJECT_0 )
    {
        if( WAIT_ABANDONED == result )
        {
            throw ApiError( "WaitForSingleObject() returned WAIT_ABONDANED" );
        }
        else if( WAIT_TIMEOUT == result )
        {
            throw ApiError( "WaitForSingleObject() returned WAIT_TIMEOUT" );
        }
        else if( WAIT_FAILED == result )
        {
            throw ApiError( "WaitForSingleObject() returned WAIT_FAILED", GetLastError() );
        }
        else
        {
            throw ApiError( "WaitForSingleObject() returned undocumented value" );
        }
    }
}

void ManualResetEvent::setSignaled( bool signaledState ) throw( ApiError )
{
    if( signaledState )
    {
        set();
    }
    else
    {
        reset();
    }
}

void ManualResetEvent::set() throw( ApiError )
{
    BOOL result;

    result = SetEvent( handle );
    if( FALSE == result )
    {
        throw ApiError( "SetEvent() returned NULL", GetLastError() );
    }
}

void ManualResetEvent::reset() throw( ApiError )
{
    BOOL result;

    result = ResetEvent( handle );
    if( FALSE == result )
    {
        throw ApiError( "ResetEvent() returned NULL", GetLastError() );
    }
}
