#include "Logger.h"
#include <fstream>
#include <memory>
#include <iostream>
#include <cassert>

using namespace std;

Logger::Logger( ostream * ost )
: mux( NULL ), ost( ost )
{
    mux = CreateMutex( NULL, FALSE, NULL );
}

Logger::~Logger()
{
    CloseHandle( mux );
}

void Logger::println( const string & line )
{
    WaitForSingleObject( mux, INFINITE );

    if( NULL != ost )
    {
        ( * ost ) << line << endl;
    }

    ReleaseMutex( mux );
}

void Logger::println( ostringstream & line )
{
    WaitForSingleObject( mux, INFINITE );

    if( NULL != ost )
    {
        ( * ost ) << line.str() << endl;
    }
    line.str( "" );

    ReleaseMutex( mux );
}

Logger & Logger::getInstance( const string & name )
{
    static map<string, Logger *> loggers;
    map<string, Logger *>::iterator iter;

    if( loggers.empty() )
    {
        init( loggers );
    }

    iter = loggers.find( name );
    if( loggers.end() == iter )
    {
        iter = loggers.find( "" );
    }

    assert( iter != loggers.end() );

    return * iter->second;
}

void Logger::init( map<string, Logger *> & loggers )
{
    static Logger loggersArray[] = {
        Logger( NULL ),
        Logger( & cerr )
    };

    loggers.insert( make_pair( "", loggersArray + 0 ) );
//    loggers.insert( make_pair( "AutoResetEvent debug", loggersArray + 1 ) );
    loggers.insert( make_pair( "tests", loggersArray + 1 ) );
//    loggers.insert( make_pair( "work distrib debug", loggersArray + 1 ) );
}
