#ifndef LOGSTREAM_H_INCLUDED
#define LOGSTREAM_H_INCLUDED

#include <ostream>
#include <string>

class LogStream
{
private:

    LogStream( Stream * str );

public:

    typedef std::ostream Stream;
    typedef std::string String;

    ~LogStream();

    template < typename T >
    LogStream & operator<<( const T & value );

    static LogStream * createInstance( String fileName );

private:

    Stream * stream;
    HANDLE muxHandle;
};

LogStream::LogStream( Stream * str )
: stream( str ), muxHandle( NULL )
{
    muxHandle = CreateMutex(
        NULL /* security */, FALSE /* initial owner */, NULL /* name */
    );
}

LogStream::~LogStream()
{
    delete stream;
}

template < typename T >
LogStream & operator<<( const T & value )
{
    WaitForSingleObject( muxHandle, INFINITE );

    ( * stream ) << value;

    ReleaseMutex( muxHandle );

    return stream;
}

#endif // LOGSTREAM_H_INCLUDED
