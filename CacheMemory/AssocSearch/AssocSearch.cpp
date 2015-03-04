#ifdef INCLUDED_AS_HEADER

#include "AssocSearch.h"
#include "windows.h"
#include <cassert>
#include "../Logger/Logger.h"

using namespace std;

template < typename Iter>
AssocSearch<Iter>::AssocSearch( Iter begin, Iter end, size_t threadCount )
    throw( InvalidArgument, RuntimeError )
: beginColl( begin ), endColl( end ),
    threadCount( threadCount ), threads( NULL ), threadArgs( NULL ),
    sync( NULL ), searchQueryIsReady( NULL ), searchResultIsReady( NULL ),
    resultRegisteringMux( NULL ), result(),
    exitFlag( false ), foundFlag( false )
{
    // check argument

    if( threadCount <= 0 )
    {
        throw InvalidArgument( "Thread count must be a positive integer" );
    }

    // create synchonization objects

    try
    {
        sync = new Synchronize( threadCount );
        searchQueryIsReady = new AutoResetEvent( false );
        searchResultIsReady = new AutoResetEvent( false );
        resultRegisteringMux = new Mutex();
    }
    catch( ApiError & ex )
    {
        throw RuntimeError( ex.what() );
    }

    // create thread arguments

    threadArgs.resize( threadCount );
    for( size_t i = 0; i < threadCount; ++i )
    {
        threadArgs[ i ].pSelf = this;
        threadArgs[ i ].manager = false;
        threadArgs[ i ].beginInterval = threadArgs[ i ].beginInterval = end;
    }
    threadArgs.front().manager = true;

    // create threads

    threads = new uintptr_t[ threadCount ];

    // create and start threads

    uintptr_t result = 0;
    for( size_t i = 0; i < threadCount; ++i )
    {
        result = _beginthreadex(
            NULL /* serurity */,
            0 /* stack size */,
            threadFunction /* start address */,
            & threadArgs[ i ] /* arguments */,
            0 /* initial state (0 for running)*/,
            NULL /* recieves thread identifier */
        );

        if( 0 == result )
        {
            break;
        }

        threads[ i ] = result;
    }

    if( 0 == result )
    {
        // TODO implement particular destruction

        ApiError apiEx( "_beginthreadex() returned 0" );
        throw RuntimeError( apiEx.what() );
    }

    setCollection( begin, end );
}

template < typename Iter >
AssocSearch<Iter>::~AssocSearch()
{
    // TODO implement destruction
}

template < typename Iter >
void AssocSearch<Iter>::setCollection( const Iter beginColl,
                                      const Iter endColl )
{
    Logger & log = Logger::getInstance( "work distrib debug" );
    ostringstream osstr;

    // set fields values
    this->beginColl = beginColl;
    this->endColl = endColl;

    size_t collSize = endColl - beginColl;

    osstr << "Collection: ";
    for( Iter iter = beginColl; iter != endColl; ++iter )
    {
        osstr << "(" << * iter << ") ";
    }
    log.println( osstr );

    // reallot intervals to threads
    for( size_t iThread = 0; iThread < threadCount; ++iThread )
    {
        size_t iStart = iThread * collSize / threadCount,
            iEnd = ( iThread + 1 ) * collSize / threadCount;

        threadArgs[ iThread ].beginInterval = beginColl + iStart;
        threadArgs[ iThread ].endInterval = beginColl + iEnd;

        osstr << "Thread " << iThread << ": ";
        osstr << "iStart = " << iStart;
        osstr << " iEnd = " << iEnd;
        for( Iter iter = threadArgs[ iThread ].beginInterval;
            iter != threadArgs[ iThread ].endInterval; ++iter )
        {
            osstr << "(" << * iter << ") ";
        }
        log.println( osstr );
    }
}

template < typename Iter >
Iter AssocSearch<Iter>::findIf( Predicate * pred ) throw( RuntimeError )
{
    try
    {
        // initialize search result
        result = endColl;

        // put predicate to thread arguments
        for( size_t iThread = 0; iThread < threadCount; ++iThread )
        {
            threadArgs[ iThread ].pred = pred;
        }

        // start search
        searchQueryIsReady->setSignaled( true );

        // wait result of search
        searchResultIsReady->waitAndReset();
    }
    catch( exception & ex )
    {
        throw RuntimeError( ex.what() );
    }

    return result;
}

template < typename Iter >
unsigned WINAPI assocSearchThreadFunction( void * pArgs )
{
    typedef AssocSearch<Iter> Ass;
    typedef typename Ass::ThreadArguments Args;

    Args * args = static_cast<Args *>( pArgs );
    Ass * pSelf = args->pSelf;

    return pSelf->threadMethod( args );
}

template < typename Iter >
typename AssocSearch<Iter>::ThreadFunctionType * AssocSearch<Iter>::threadFunction = & assocSearchThreadFunction<Iter>;

template < typename Iter >
unsigned AssocSearch<Iter>::threadMethod( ThreadArguments * pArgs )
{
    bool iAmManager = pArgs->manager;
    Iter & beginInterval = pArgs->beginInterval,
        & endInterval = pArgs->endInterval;
    Predicate *& pred = pArgs->pred;

    while( ! exitFlag )
    {
        // thread-manager wait query from main thread
        if( iAmManager )
        {
            searchQueryIsReady->waitAndReset();
        }

        // threads wait one others
        sync->operator()();

        // search
        for( Iter iter = beginInterval;
            ( iter != endInterval ) && ( ! foundFlag ); ++iter )
        {
            if( pred( * iter ) )
            {
                resultRegisteringMux->waitAndOwn();
                {
                    result = iter;
                    foundFlag = true;
                }
                resultRegisteringMux->release();
            }
        }

        // threads wait one others
        sync->operator()();

        // thread-manager tells about ready result
        if( iAmManager )
        {
            searchResultIsReady->setSignaled( true );
        }
    }

    return 0;
}

template < typename Iter >
AssocSearch<Iter> makeAccocSearch( Iter begin, Iter end, size_t threadCount )
{
    return AssocSearch<Iter>( begin, end, threadCount );
}

#endif // INCLUDED_AS_HEADER
