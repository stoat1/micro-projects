#ifndef FINDIFALL_H_INCLUDED
#define FINDIFALL_H_INCLUDED

#include <stdexcept>
#include <vector>
#include "WinapiException.h"
#include "windows.h"
#include "Synchronize.h"
#include "AutoResetEvent.h"
#include "ManualResetEvent.h"
#include "Mutex.h"

template < typename Iter >
unsigned WINAPI assocSearchThreadFunction( void * );

template < typename Iter >
class AssocSearch
{
private:

    // object is not copyable
    AssocSearch( const AssocSearch<Iter> & );
    AssocSearch & operator=( const AssocSearch<Iter> & );

public:

    // exception classes

    class InvalidArgument : public std::invalid_argument
    {
    public:

        InvalidArgument( const std::string & what )
        : std::invalid_argument( what )
        {}
    };

    class RuntimeError : public std::runtime_error
    {
    public:

        RuntimeError( const std::string & what )
        : std::runtime_error( what )
        {}
    };

    // type of function which is userd for search as a condition
    typedef bool Predicate( const typename Iter::value_type & );


    // constructor
    explicit AssocSearch( Iter begin, Iter end, size_t threadCount )
        throw( InvalidArgument, RuntimeError );

    // destructor
    virtual ~AssocSearch();

    // change collection to search in
    void setCollection( Iter begin, Iter end );

    // find elem : pred( elem ) == true
    Iter findIf( Predicate * pred ) throw( RuntimeError );

    friend unsigned WINAPI assocSearchThreadFunction<Iter>( void * );

private:

    struct ThreadArguments
    {
    public:

        ThreadArguments()
        : pSelf( NULL ), pred( NULL ), manager( false ),
            beginInterval(), endInterval()
        {}

        AssocSearch<Iter> * pSelf;
        Predicate * pred;

        bool manager;
        Iter beginInterval, endInterval;
    };

    // thread function
    typedef unsigned WINAPI (ThreadFunctionType)( void * );
    static ThreadFunctionType * threadFunction;
    unsigned threadMethod( ThreadArguments * );

    // collection
    Iter beginColl, endColl;

    // array of thread handles
    size_t threadCount;
    uintptr_t * threads;
    std::vector<ThreadArguments> threadArgs;

    // syncrhonization functor
    Synchronize * sync;

    // some synchronization objects
    AutoResetEvent * searchQueryIsReady,
        * searchResultIsReady;
    Mutex * resultRegisteringMux;

    // threads put its search result here
    Iter result;

    // flag which tells to theads to return from thread function
    bool exitFlag;

    // flag which tells to threads to stop curent searching
    bool foundFlag;
};

template < typename Iter >
AssocSearch<Iter> makeAccocSearch( Iter begin, Iter end, size_t threadCount );

#ifdef INCLUDED_AS_HEADER
#   error identifier INCLUDED_AS_HEADER is already occupied
#endif

#define INCLUDED_AS_HEADER
#   include "AssocSearch.cpp"
#undef INCLUDED_AS_HEADER

#endif // FINDIFALL_H_INCLUDED
