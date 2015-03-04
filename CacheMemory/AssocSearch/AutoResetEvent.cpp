#include "AutoResetEvent.h"

using namespace std;

#include "../Logger/Logger.h"

AutoResetEvent::AutoResetEvent( bool signaledState ) throw( ApiError )
: handle( NULL )
{
    Logger & log = Logger::getInstance( "AutoResetEvent debug" );
    ostringstream osstr;

    osstr << "In AutoReestEvent[" << this << "]::AutoResetEvent(bool)";
    log.println( osstr );

    BOOL bInitialState = ( signaledState ? TRUE : FALSE );

    handle = CreateEvent(
        NULL /* security attributes */,
        FALSE /* manual reset */,
        bInitialState /* initial state is signaled flag */,
        NULL /* name */
    );

    osstr << "CreateEvent(NULL, FALSE, " << bInitialState
        << ", NULL) returned " << handle;
    log.println( osstr );
    osstr.str( "" );

    if( NULL == handle )
    {
        throw ApiError( "CreateEvent() returned NULL", GetLastError() );
    }
}

AutoResetEvent::~AutoResetEvent()
{
    Logger & log = Logger::getInstance( "AutoResetEvent debug" );
    ostringstream osstr;

    osstr << "In AutoReestEvent[" << this << "]::~AutoResetEvent()";
    log.println( osstr );

    BOOL result;

    result = CloseHandle( handle );
    if( FALSE == result )
    {
        // TODO write about this error to log file
    }

    osstr << "CloseHandle(" << handle << ") returned " << result;
    log.println( osstr );
}

void AutoResetEvent::waitAndReset() throw( ApiError )
{
    Logger & log = Logger::getInstance( "AutoResetEvent debug" );
    ostringstream osstr;

    osstr << "In AutoReestEvent[" << this << "]::waitAndReset()";
    log.println( osstr );

    DWORD result;

    result = WaitForSingleObject( handle, INFINITE );

    osstr << "WaitForSingleObject(" << handle << ", " << INFINITE <<") returned " << result;
    log.println( osstr );

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

void AutoResetEvent::setSignaled( bool signaledState ) throw( ApiError )
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

void AutoResetEvent::set() throw( ApiError )
{
    BOOL result;

    result = SetEvent( handle );
    if( FALSE == result )
    {
        throw ApiError( "SetEvent() returned NULL", GetLastError() );
    }
}

void AutoResetEvent::reset() throw( ApiError )
{
    BOOL result;

    result = ResetEvent( handle );
    if( FALSE == result )
    {
        throw ApiError( "ResetEvent() returned NULL", GetLastError() );
    }
}
