#ifndef SYNCHRONIZE_H_INCLUDED
#define SYNCHRONIZE_H_INCLUDED

//#define TWO_EVENTS_IMPLEMENTATION

#include "windows.h"
#include <stdexcept>

#ifdef TWO_EVENTS_IMPLEMENTATION
#   include "AutoResetEvent.h"
#else
#   include "ManualResetEvent.h"
#   include "Mutex.h"
#endif

#include <memory>

class Synchronize
{
public:

    class InvalidArgument : public std::invalid_argument
    {
    public:

        explicit InvalidArgument( const std::string & what )
        : std::invalid_argument( "Synchonize::InvalidArugment: " + what )
        {}
    };

    class RuntimeError : public std::runtime_error
    {
    public:

        explicit RuntimeError( const std::string & what )
        : std::runtime_error( "Synchronize::RuntimeError: " + what ) {}
    };

    Synchronize( size_t count ) throw( RuntimeError, InvalidArgument );
    virtual ~Synchronize();

    void operator()() throw( RuntimeError );

private:

    size_t const totalCount;
    size_t insideCount;

#   ifdef TWO_EVENTS_IMPLEMENTATION
    std::auto_ptr<AutoResetEvent> pAutoEvent1, pAutoEvent2;
#   else
    std::auto_ptr<Mutex> pMux1, pMux2;
    std::auto_ptr<ManualResetEvent> pEvent1, pEvent2;
#   endif
};

#endif // SYNCHRONIZE_H_INCLUDED
