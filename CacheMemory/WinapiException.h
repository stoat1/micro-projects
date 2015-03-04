#ifndef WINAPIEXCEPTION_H_INCLUDED
#define WINAPIEXCEPTION_H_INCLUDED

#include "windows.h"
#include <stdexcept>

class ApiError : public std::runtime_error
{
public:

    ApiError( const std::string & what );
    ApiError( const std::string & what, int errnoValue, int _dosrrnoValue );
    ApiError( const std::string & what, int errnoValue );
    ApiError( const std::string & what, DWORD getLastErrorValue );
};

#endif // WINAPIEXCEPTION_H_INCLUDED
