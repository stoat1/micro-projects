#include <cmath>

#include "ParetoRequests.h"

using namespace std;

ParetoRequests::ParetoRequests(
    size_t storageSize,
    size_t dequeSize,
    size_t minGroupSize,
    double alpha
) throw( invalid_argument )
: storageSize( storageSize )
, pareto( minGroupSize, alpha )
, rand( 0 )
, files()
{
    if( storageSize <= 0 )
    {
        throw invalid_argument( "ParetoRequests: storageSize must be positive" );
    }

//    if( dequeSize < 0 )
//    {
//        throw invalid_argument( "ParetoRequests: dequeSize must not be negative" );
//    }

    while( files.size() < dequeSize )
    {
        File file;
        createFile( file );

        files.push_back( file );
    }
}

ParetoRequests::~ParetoRequests()
{}

MemRequest ParetoRequests::nextRequest()
{
    static deque<MemRequest> buffer;

    if( buffer.empty() )
    {
        // add read requests to files from deque
        for( deque<File>::iterator iFile = files.begin();
            iFile != files.end(); ++iFile )
        {
            MemRequest req(
                iFile->address,
                iFile->address + iFile->size,
                MemRequest::READING
            );

            buffer.push_back( req );
        }

        // push back and pop front
        File file;
        createFile( file );
        files.pop_front();
        files.push_back( file );

        // add write request to new pushed file
        MemRequest req(
            file.address,
            file.address + file.size,
            MemRequest::WRITING
        );
        buffer.push_back(req);
    }

    MemRequest req = buffer.front();
    buffer.pop_front();

    return req;
}

void ParetoRequests::createFile( File & file )
{
    file.size = ceil( pareto.nextValue() );
    file.size = min( file.size, storageSize - 1 );
    file.size = max( file.size, static_cast<size_t>( 1 ) );

    file.address = rand.nextValue() % ( storageSize - file.size );
}
