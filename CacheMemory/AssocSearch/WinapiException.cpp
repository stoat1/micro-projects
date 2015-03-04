#include "WinapiException.h"
#include <sstream>

using namespace std;

template < typename T >
static string toString( const T & value )
{
    ostringstream osst;

    osst << value;
    return osst.str();
}

ApiError::ApiError( const string & whatArg )
: runtime_error( whatArg )
{}

ApiError::ApiError( const string & whatArg,
                   int errnoValue, int _doserrnoValue )
: runtime_error( "ApiError: " + whatArg + ", errno=" + strerror( errnoValue ) + ", _doserrno=" + strerror( _doserrnoValue ) )
{}

ApiError::ApiError( const string & whatArg, int errnoValue )
: runtime_error( "ApiError: " + whatArg + ", errno=" + strerror( errnoValue ) )
{}

ApiError::ApiError( const string & whatArg, DWORD getLastErrorValue )
: runtime_error( "ApiError" + whatArg + ", GetLastError()=" + toString( getLastErrorValue ) )
{}
