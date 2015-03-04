#include "Mutex.h"

Mutex::Mutex() throw( ApiError )
: handle( NULL )
{
    handle = CreateMutex(
        NULL /* security */,
        FALSE /* initial owner */,
        NULL /* name */
    );

    if( NULL == handle )
    {
        throw ApiError( "CreateMutex() returned NULL", GetLastError() );
    }
}

Mutex::~Mutex()
{
    BOOL result;

    result = CloseHandle( handle );
    if( FALSE == result )
    {
        // TODO write about this error to log file
    }
}

void Mutex::waitAndOwn() throw( ApiError )
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

void Mutex::release() throw( ApiError )
{
    BOOL result;

    result = ReleaseMutex( handle );
    if( FALSE == result )
    {
        throw ApiError( "ReleaseMutex() returned FALSE", GetLastError() );
    }
}
