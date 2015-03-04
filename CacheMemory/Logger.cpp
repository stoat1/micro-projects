#include "Logger.h"
#include <fstream>
#include <memory>
#include <iostream>
#include <fstream>
#include <cassert>

using namespace std;

Logger::Logger( const string & name, ostream * ost )
: name( name ), mux( NULL ), ost( ost )
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
        ( * ost ) << name << " : " << line << endl;
    }

    ReleaseMutex( mux );
}

void Logger::println( ostringstream & line )
{
    WaitForSingleObject( mux, INFINITE );

    if( NULL != ost )
    {
        ( * ost ) << name << " : " << line.str() << endl;
    }
    line.str( "" );

    ReleaseMutex( mux );
}

Logger & Logger::getInstance( const string & name )
{
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

void Logger::createFileLogger( const string & loggerName, const string & fileName )
{
    ofstream * logFile = new ofstream;
    logFile->open( fileName.c_str(), ios_base::out );
    loggers.insert( make_pair( loggerName, new Logger( loggerName, logFile ) ) );
}

void Logger::createStreamLogger( const string & loggerName, ostream * ostr )
{
    loggers.insert( make_pair( loggerName, new Logger( loggerName, ostr ) ) );
}

void Logger::init( map<string, Logger *> & loggers )
{
    static Logger loggersArray[] = {
        Logger( "null", NULL ),
        Logger( "CacheGroup log", & cerr ),
        Logger( "CachedMemory log", & cerr ),
        Logger( "tests log", & cerr ),
        Logger( "FindEntryFunctor log", & cerr ),
        Logger( "CategoricalRequests log", & cerr )
    };

    loggers.insert( make_pair( "", loggersArray + 0 ) );
//    loggers.insert( make_pair( "AutoResetEvent debug", loggersArray + 1 ) );
    loggers.insert( make_pair( "tests", loggersArray + 3 ) );
//    loggers.insert( make_pair( "work distrib debug", loggersArray + 1 ) );
//    loggers.insert( make_pair( "FindEntryFunctor debug", loggersArray + 4 ) );
//    loggers.insert( make_pair( "CategoricalRequests debug", loggersArray + 5 ) );
}

map<string, Logger *> Logger::loggers;
