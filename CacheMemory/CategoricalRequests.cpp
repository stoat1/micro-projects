#include "CategoricalRequests.h"
#include <cstdlib>
#include <cassert>
#include "Logger.h"
#include "NormalDistribution.h"

using namespace std;

CategoricalRequests::CategoricalRequests(
    size_t storageSize,
    size_t catSize,
    double reRequestProb
) throw( invalid_argument )
: storageSize( storageSize )
, cats()
, recentCats()
, random( 0 )
, reRequestProb( reRequestProb )
, uniform( new UniformDistribution( Random( 0 ) ) )
{
    Logger & log = Logger::getInstance( "CategoricalRequests debug" );
    ostringstream osstr;

    osstr << "In CategoricalRequests::CategoricalRequests(storageSize="
        << storageSize << ")";
    log.println( osstr );

    if( storageSize <= 0 )
    {
        throw invalid_argument( "storageSize must be a positive integer" );
    }

    address_t nextAddress = 0;
    while( nextAddress <= storageSize )
    {
        cats.push_back( Category( nextAddress, nextAddress + catSize ) );
        nextAddress += catSize;
    }

    cats.back().end = storageSize;

    osstr << "cats={" << endl;
    for( size_t iCat = 0; iCat < cats.size(); ++iCat )
    {
        osstr << "\t" << "(" << cats[iCat].begin;
        osstr << ", " << cats[iCat].end << ")" << endl;
    }
    osstr << "}";
    log.println( osstr );
}

CategoricalRequests::~CategoricalRequests()
{
}

MemRequest CategoricalRequests::nextRequest()
{
    double re = ( recentCats.size() * reRequestProb
        + ( cats.size() - recentCats.size() ) * ( 1 - reRequestProb ) ) / cats.size();

    if( re < 0 ) re = 0;
    if( re > 1 ) re = 1;

    if( random.nextBoolean( re ) && ! recentCats.empty() )
    {
        // request again to old category
        Category cat = recentCats.front();
        if( ! random.nextBoolean( re ) )
        {
            recentCats.pop_front();
        }

        return MemRequest( cat.begin, cat.end, MemRequest::READING );
    }
    else
    {
        // request to new category
        Category cat = cats.at( random.nextValue() % cats.size() );
        recentCats.push_back( cat );

        return MemRequest( cat.begin, cat.end, MemRequest::WRITING );
    }
}

//CategoricalRequests::Category & CategoricalRequests::selectCategory()
//{
//    double t = uniform->nextValue() * cats.back().cumulativeProb;
//
//    for( size_t iCat = 0; iCat < cats.size(); ++iCat )
//    {
//        if( t < cats[ iCat ].cumulativeProb )
//            return cats[ iCat ];
//    }
//
//    return cats.back();
//}

//size_t CategoricalRequests::generateCatSize()
//{
//    return random.nextValue() % 10 + 1;
//}

//double CategoricalRequests::generateCatProb()
//{
//    double t = catsProbDistrib->nextValue();
//
//    return ( t > 0 ? t : -t );
//}
