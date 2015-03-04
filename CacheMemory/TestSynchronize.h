#ifndef TESTSYNCHRONIZE_H_INCLUDED
#define TESTSYNCHRONIZE_H_INCLUDED

#include <iostream>
#include <string>
#include <cstdlib>
#include <stdexcept>
#include "Synchronize.h"
#include "windows.h"
#include "process.h"
#include "time.h"
#include "Mutex.h"

using namespace std;

struct Args
{
    Args( Synchronize * s, Mutex * m, bool * ef, DWORD d )
    : sync( s ), mux( m ), exitFlag( ef ), delay( d )
    {}

    Synchronize * sync;
    Mutex * mux;
    bool * exitFlag;
    DWORD delay;
};

DWORD delays[] = { 0, 11, 101, 1001 };
const size_t N = sizeof( delays ) / sizeof( DWORD );
//Synchronize sync( N );

static unsigned WINAPI threadFunction( void * arg )
{
    Args * args = static_cast<Args *>( arg );

    try
    {
        while( ! ( * args->exitFlag ) )
        {
            Sleep( args->delay );

            args->sync->operator()();

            args->mux->waitAndOwn();
            cout << "delay = " << args->delay << ", \tclock() = " << clock() << endl;
            args->mux->release();


        }

        args->mux->waitAndOwn();
        cout << "delay = " << args->delay << " \texiting" << endl;
        args->mux->release();

        return 0;
    }
    catch( exception & ex )
    {
        cout << "Exception: " << ex.what() << endl;

        return -1;
    }
}

void testSynchronize()
{
    Synchronize * sync = NULL;
    Mutex * mux = NULL;
    bool * exitFlag = NULL;

    try
    {
        sync = new Synchronize( N );
        mux = new Mutex();
        exitFlag = new bool(false);

        uintptr_t threads[ N ];

        for( size_t i = 0; i < N; ++i )
        {
            Args * args = new Args( sync, mux, exitFlag, delays[ i ] );

            threads[ i ] = _beginthreadex(
                NULL,
                0,
                & threadFunction,
                args,
                0,
                NULL
            );
        }

        Sleep( 4000 );
        * exitFlag = true;

        WaitForMultipleObjects( N, reinterpret_cast<void* const*>( threads ), TRUE, INFINITE );
    }
    catch( exception & ex )
    {
        cout << "Error: " << ex.what() << endl;
    }
}

#endif // TESTSYNCHRONIZE_H_INCLUDED
