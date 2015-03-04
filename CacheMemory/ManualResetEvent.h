#ifndef MANUALEVENT_H_INCLUDED
#define MANUALEVENT_H_INCLUDED

#include "windows.h"
#include "WinapiException.h"

class ManualResetEvent
{
public:

    explicit ManualResetEvent( bool signaledState ) throw( ApiError );
    virtual ~ManualResetEvent();

    void wait() throw( ApiError );
    void setSignaled( bool signaledState ) throw( ApiError );

private:

    void set() throw( ApiError );
    void reset() throw( ApiError );

    ManualResetEvent( const ManualResetEvent & orig );
    ManualResetEvent & operator=( const ManualResetEvent & orig );

    HANDLE handle;
};

#endif // MANUALEVENT_H_INCLUDED
