#ifndef MUTEX_H_INCLUDED
#define MUTEX_H_INCLUDED

#include "windows.h"
#include "WinapiException.h"

class Mutex
{
public:

    Mutex() throw( ApiError );
    virtual ~Mutex();

    void waitAndOwn() throw( ApiError );
    void release() throw( ApiError );

private:

    Mutex( const Mutex & orig );
    Mutex & operator=( const Mutex & orig );

    HANDLE handle;
};

#endif // MUTEX_H_INCLUDED
