#ifndef AUTORESETEVENT_H_INCLUDED
#define AUTORESETEVENT_H_INCLUDED

#include "windows.h"
#include "WinapiException.h"

#define DEBUG_AUTORESETEVENT

#ifdef DEBUG_AUTORESETEVENT
#   include <iostream>
#endif

class AutoResetEvent
{
public:

    explicit AutoResetEvent( bool signaledState ) throw( ApiError );
    virtual ~AutoResetEvent();

    void waitAndReset() throw( ApiError );
    void setSignaled( bool signaledState ) throw( ApiError );

private:

    void set() throw( ApiError );
    void reset() throw( ApiError );

    AutoResetEvent( const AutoResetEvent & orig );
    AutoResetEvent & operator=( const AutoResetEvent & orig );

    HANDLE handle;
};

#endif // AUTORESETEVENT_H_INCLUDED
