#include "CachedMemory.h"

#include <cassert>
#include <algorithm>
#include <functional>

#ifdef __DEBUG__
#   include <iostream>
#   include <sstream>
#endif

#define DO_NOT_WRITE_DIRECTLY

#include <stdexcept>

using namespace std;

// constructor
CachedMemory::CachedMemory( AbstractMemory & mainMem,
                    size_t cacheSize, size_t groupWidth, size_t flagResetFreq )
: GROUP_WIDTH( groupWidth )
, GROUPS( 1 << GROUP_WIDTH )
, GROUP_SIZE( cacheSize / GROUPS )
, mainMemory( mainMem )
, cache()
{
    osstr << "In CachedMemory::CachedMemory(mainMem=&"
        << static_cast<void *>( & mainMem ) << ", cachedSize="
        << cacheSize << ", groupWidth=" << groupWidth << ")";
    log.println( osstr );

    osstr << "GROUPS = " << GROUPS << endl;
    osstr << "GROUP_SIZE = " << GROUP_SIZE;
    log.println( osstr );

    // chech arguments

    if( cacheSize <= 0 )
    {
        throw invalid_argument( "Cache size must be a positive integer" );
    }

    if( GROUP_SIZE <= 0 )
    {
        ostringstream osstr;
        osstr << "Cannot create cache with size = " << cacheSize
            << " and gropu width = " << groupWidth;
        throw invalid_argument( osstr.str() );
    }

    // create entries

    cache.resize( GROUPS - 1 );
    for( Cache::iterator iter = cache.begin();
        iter != cache.end(); ++iter )
    {
        * iter = new Group( GROUP_SIZE, flagResetFreq );
    }
    size_t lastGroupSize = cacheSize - ( GROUPS - 1 ) * GROUP_SIZE;
    cache.push_back( new Group( GROUP_SIZE, flagResetFreq ) );
}

// destructor
CachedMemory::~CachedMemory()
{
    for( Cache::iterator iter = cache.begin();
        iter != cache.end(); ++iter )
    {
        delete * iter;
    }
}

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
CachedMemory::value_t CachedMemory::get( address_t address ) throw( range_error )
{
    throwIfOutOfRange( address );

    osstr << "In CachedMemory::get(address=" << address << ")";
    log.println( osstr );

    group_index_t groupIndex = whichGroup( address );
    tag_t tag = getTag( address );
    bool hit;
    value_t value;

    osstr << "groupIndex = " << groupIndex;
    log.println( osstr );

    Group & g = * cache[ groupIndex ];

    Group::Iterator iter = g.callEntryIterator( tag, hit );
    if( hit )
    {
        log.println( "hit" );

        value = iter.getValue();
    }
    else
    {
        log.println( "miss" );

        // if entry is dirty
        if( iter.isDirty() )
        {
            value_t writeBackValue = iter.getValue();
            address_t writeBackAddress = iter.getTag(); // FIXME tag is not address

            osstr << "writeback value " << writeBackValue
                << " to address " << writeBackAddress;
            log.println( osstr );

            // write old entry value back to main memory
            mainMemory.set( writeBackAddress, writeBackValue );
        }

        // read from main memory
        value = mainMemory.get( address );

        osstr << "loaded value = " << value;
        log.println( osstr );

        // make entry
        iter.setTagAndValue( tag, value );
    }

    return value;
}

// write
void CachedMemory::set( address_t address, value_t value ) throw( range_error )
{
    throwIfOutOfRange( address );

    osstr << "In CachedMemory::set(address=" << address
        << ", value=" << value << ")";
    log.println( osstr );

    group_index_t groupIndex = whichGroup( address );
    tag_t tag = getTag( address );
    bool hit;

    osstr << "groupIndex = " << groupIndex;
    log.println( osstr );

    Group & g = * cache[ groupIndex ];

    Group::Iterator iter = g.callEntryIterator( tag, hit );
    if( hit )
    {
        log.println( "hit" );

        iter.setValue( value );
    }
    else
    {
        log.println( "miss" );

        // if entry is dirty
        if( iter.isDirty() )
        {
            value_t writeBackValue = iter.getValue();
            address_t writeBackAddress = iter.getTag(); // FIXME tag is not address

            osstr << "writeback value " << writeBackValue
                << " to address " << writeBackAddress;
            log.println( osstr );

            // write old entry value back to main memory
            mainMemory.set( writeBackAddress, writeBackValue );
        }

#       ifdef DO_NOT_WRITE_DIRECTLY
        iter.setTagAndValue( tag, value );
        iter.setValue( value );
#       else
        osstr << "save value " << value << " to address " << address;
        log.println( osstr );

        // write to main memory
        mainMemory.set( address, value );

        // make entry
        iter.setTagAndValue( tag, value );
#       endif
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
        osst << ** it;
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

Logger & CachedMemory::log = Logger::getInstance( "CachedMemory debug" );
ostringstream CachedMemory::osstr;
