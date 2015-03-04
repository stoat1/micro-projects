#include "CachedMemory.h"

#include <cassert>
#include <algorithm>
#include <functional>

#ifdef __DEBUG__
#   include <iostream>
#   include <sstream>
#endif

#include <stdexcept>

using namespace std;

// constructor
CachedMemory::CachedMemory( AbstractMemory & mainMem, size_t cacheSize, size_t groupWidth )
: GROUP_WIDTH( groupWidth ), GROUPS( 1 << GROUP_WIDTH ),
    GROUP_SIZE( cacheSize / GROUPS ),
    mainMemory( mainMem ), cache()
{
    // chech arguments

    if( cacheSize <= 0 )
    {
        throw invalid_argument( "Cache size must be a positive integer" );
    }

    if( GROUP_SIZE <= 0 )
    {
        throw invalid_argument( "Cannot create cache becouse groupWidth given is too large" );
    }

    // create entries

    cache.resize( GROUPS - 1, Group( GROUP_SIZE, GROUP_SIZE * 4 ) );
    size_t lastGroupSize = cacheSize - ( GROUPS - 1 ) * GROUP_SIZE;
    cache.push_back( Group( GROUP_SIZE, lastGroupSize ) );
}

// destructor
CachedMemory::~CachedMemory()
{}

//// read
//CachedMemory::value_t CachedMemory::get( address_t address )
//{
//    group_index_t groupIndex = whichGroup( address );
//    tag_t tag = getTag( address );
//    bool hit;
//    value_t value;
//
//    Group & g = cache[ groupIndex ];
//
//    Group::index_t i = g.callEntry( tag, hit );
//    if( hit )
//    {
//        g.setReferenced( i, true );
//        value = g.getValue( i );
//    }
//    else
//    {
//        // if entry is dirty
//        if( g.isActual( i ) && g.isModified( i ) )
//        {
//            mainMemory.set( address, g.getValue( i ) );
//        }
//
//        // read from main memory
//        value = mainMemory.get( address );
//
//        // make entry
//        g.setActual( i, true );
//        g.setReferenced( i, true );
//        g.setModified( i, false );
//        g.setTag( i, tag );
//        g.setValue( i, value );
//    }
//
//    return value;
//}

// read
CachedMemory::value_t CachedMemory::get( address_t address )
{
    group_index_t groupIndex = whichGroup( address );
    tag_t tag = getTag( address );
    bool hit;
    value_t value;

    Group & g = cache[ groupIndex ];

    Group::Iterator iter = g.callEntryIterator( tag, hit );
    if( hit )
    {
        value = iter.getValue();
    }
    else
    {
        // if entry is dirty
        if( iter.isActual() && iter.isModified() )
        {
            mainMemory.set( address, iter.getValue() );
        }

        // read from main memory
        value = mainMemory.get( address );

        // make entry
        iter.setTagAndValue( tag, value );
    }

    return value;
}

// write
void CachedMemory::set( address_t address, value_t value )
{
    group_index_t groupIndex = whichGroup( address );
    tag_t tag = getTag( address );
    bool hit;

    Group & g = cache[ groupIndex ];

    Group::index_t i = g.callEntry( tag, hit );
    if( hit )
    {
        g.setReferenced( i, true );
        g.setModified( i, true );
        g.setValue( i, value );
    }
    else
    {
        // if entry is dirty
        if( g.isActual( i ) && g.isModified( i ) )
        {
            // write old entry value back to main memory
            mainMemory.set( address, g.getValue( i ) );
        }

        // write to main memory
        mainMemory.set( address, value );

        // make entry
        g.setActual( i, true );
        g.setReferenced( i, true );
        g.setModified( i, false );
        g.setTag( i, tag );
        g.setValue( i, value );
    }
}

// elems in array
CachedMemory::address_t CachedMemory::size() const
{
    return mainMemory.size();
}

// maps address to group
CachedMemory::group_index_t CachedMemory::whichGroup( address_t address ) const
{
//    size_t mask = ~ ( ( ~ size_t( 0 ) ) << GROUP_WIDTH );
//    return address && mask;
    return ( ( address >> GROUP_WIDTH ) << GROUP_WIDTH ) ^ address;
}

// maps address to tag
size_t CachedMemory::getTag( address_t address ) const
{
    return address;
}

#ifdef __DEBUG__
std::ostream & operator<<( std::ostream & ost, const CachedMemory & cm )
{
    ost << "CachedMemory [" << endl;

    for( CachedMemory::Cache::const_iterator it = cm.cache.begin();
        it != cm.cache.end(); ++it )
    {
        ostringstream osst;
        osst << * it;
        string str = osst.str();

        size_t pos = 0;
        while( pos != string::npos )
        {
            size_t i = str.find_first_of( '\n', pos );

            string substr;
            if( i != string::npos )
            {
                substr = str.substr( pos, i - pos );
                ost << "\t" << substr << endl;
                pos = i + 1;
            }
            else
            {
                substr = str.substr( pos );
                ost << "\t" << substr << endl;
                break;
            }
        }
    }

    ost << "]";

    return ost;
}
#endif
