#include "CacheGroup.h"
#include <cassert>
#include <algorithm>
#include <functional>
#include <stdexcept>

//#define ASSOCIATIVE_SEARCH

#ifdef ASSOCIATIVE_SEARCH
#   include "boost/thread.hpp"
#endif

using namespace std;

CacheGroup::CacheGroup( size_t size, size_t maxCount )
: entries( size, Entry() ), COUNT_MAX( maxCount ), count( 0 )
{
    // check argument
    if( size <= 0 )
    {
        throw invalid_argument( "CacheGroup size must be greater than 0" );
    }

    for( EntryVector::iterator it = entries.begin();
        it != entries.end(); ++it )
    {
//        Entry * entry = NULL;
//        void * p = malloc( sizeof( Entry ) );
//        entry = static_cast<Entry *>( p );
//        * entry = Entry();
//        initEntry( * entry );

        initEntry( * it );

        EntryClass & ec
            = getEntryClass( it->referenced, it->modified );
        ec.insert( it );
    }

    assert( classSets[ 0 ].size() + classSets[ 1 ].size() + classSets[ 2 ].size()
        + classSets[ 3 ].size() == entries.size() );
}

bool CacheGroup::isActual( index_t i ) const
{
    return entries[ i ].actual;
}

bool CacheGroup::isReferenced( index_t i ) const
{
    return entries[ i ].referenced;
}

bool CacheGroup::isModified( index_t i ) const
{
    return entries[ i ].modified;
}

CacheGroup::tag_t CacheGroup::getTag( index_t i ) const
{
    return entries[ i ].tag;
}

CacheGroup::value_t CacheGroup::getValue( index_t i ) const
{
    return entries[ i ].value;
}

void CacheGroup::setActual( index_t i, bool actual )
{
    entries[ i ].actual = actual;
}

void CacheGroup::setReferenced( index_t i, bool referenced )
{
    EntryVector::iterator it = entries.begin() + i;

    getEntryClass( it->referenced, it->modified ).erase( it );
    it->referenced = referenced;
    getEntryClass( it->referenced, it->modified ).insert( it );
}

void CacheGroup::setModified( index_t i, bool modified )
{
    EntryVector::iterator it = entries.begin() + i;

    getEntryClass( it->referenced, it->modified ).erase( it );
    it->modified = modified;
    getEntryClass( it->referenced, it->modified ).insert( it );
}

void CacheGroup::setTag( index_t i, tag_t tag )
{
    entries[ i ].tag = tag;
}

void CacheGroup::setValue( index_t i, value_t value )
{
    entries[ i ].value = value;
}

size_t CacheGroup::size() const
{
    return entries.size();
}

CacheGroup::index_t CacheGroup::callEntry( tag_t tag, bool & hit )
{
    hit = true;

    EntryVector::iterator found = findEntry( tag );
    if( found == entries.end() )
    {
        // cache miss
        hit = false;

        // find appropriate entry for replacing
        found = findFree();
        if( found == entries.end() )
        {
            found = findNRU();
        }
    }

    ++count;
    if( count > COUNT_MAX )
    {
        resetFlags();
    }

    return ( found - entries.begin() ) / sizeof( Entry * );
}

CacheGroup::Iterator CacheGroup::callEntryIterator( tag_t tag, bool & hit )
{
    hit = true;

    EntryVector::iterator found = findEntry( tag );
    if( found == entries.end() )
    {
        // cache miss
        hit = false;

        // find appropriate entry for replacing
        found = findFree();
        if( found == entries.end() )
        {
            found = findNRU();
        }
    }

    ++count;
    if( count > COUNT_MAX )
    {
        resetFlags();
    }

    return Iterator( this, found );
}

CacheGroup::EntryClass & CacheGroup::getEntryClass( bool r, bool m )
{
    return classSets[ ( r ? 2 : 0 ) || ( m ? 1 : 0 ) ];
}

CacheGroup::EntryVector::iterator
    CacheGroup::findEntry( tag_t tag )
#ifdef ASSOCIATIVE_SEARCH
{
    #error Associative search is not implemented yet.
}
#else
{
    EntryVector::iterator found =
        find_if( entries.begin(), entries.end(),
            bind2nd( ptr_fun( cmpEntryTag ), tag ) );
    return found;
}
#endif

CacheGroup::EntryVector::iterator
    CacheGroup::findFree()
{
    EntryVector::iterator found =
        find_if( entries.begin(), entries.end(),
            ptr_fun( entryIsNotActual ) );
    return found;
}

CacheGroup::EntryVector::iterator
    CacheGroup::findNRU()
{
    int i;

    for( i = 0; i < 4; ++i )
    {
        if( ! classSets[ i ].empty() )
            break;
    }
    assert( i != 4 );

    EntryClass & ec = classSets[ i ];
    return getRandomElem( ec );
}

void CacheGroup::resetFlags()
{
    for( EntryVector::iterator it = entries.begin();
        it != entries.end(); ++it )
    {
        it->referenced = false;
    }
}

void CacheGroup::initEntry( Entry & entry )
{
    entry.actual = false;
    entry.referenced = false;
    entry.modified = false;
    entry.tag = tag_t();
    entry.value = value_t();
}

bool CacheGroup::cmpEntryTag( Entry e, tag_t t )
{
    return e.actual && ( e.tag == t );
}

bool CacheGroup::entryIsNotActual( const Entry & e )
{
    return ( ! e.actual );
}

CacheGroup::EntryVector::iterator
    CacheGroup::getRandomElem( EntryClass & ec )
{
    EntryClass::iterator it = ec.begin();
//    advance<EntryClass::iterator, size_t>( it, rand() % ec.size() );
    return * it;
}

#ifdef __DEBUG__
std::ostream & operator<<( std::ostream & ost, const CacheGroup & cg )
{
    ost << "CacheGroup [";

    for( size_t i = 0; i < cg.size(); ++i )
    {
        if( cg.isActual( i ) )
        {
            ost << "\t" << i << " : " << hex << cg.getTag( i ) << " "
                << dec << static_cast<int>( cg.getValue( i ) );
            if( cg.isReferenced( i ) )
                ost << " r";
            if( cg.isModified( i ) )
                ost << " m";
            ost << endl;
        }
    }

    ost << endl << "]";

    return ost;
}
#endif

CacheGroup::Iterator::Iterator( CacheGroup * group, EntryVector::iterator it )
: group( group ), iter( it )
{}

bool CacheGroup::Iterator::isActual() const
{
    return iter->actual;
}

bool CacheGroup::Iterator::isReferenced() const
{
    return iter->referenced;
}

bool CacheGroup::Iterator::isModified() const
{
    return iter->modified;
}

CacheGroup::value_t CacheGroup::Iterator::getValue()
{
    group->getEntryClass( iter->referenced, iter->modified ).erase( iter );
    iter->referenced = true;
    return iter->value;
}

void CacheGroup::Iterator::setValue( value_t value )
{
    iter->modified = true;
    group->getEntryClass( iter->referenced, iter->modified ).insert( iter );
    iter->value = value;
}

void CacheGroup::Iterator::setTagAndValue( tag_t tag, value_t value )
{
    group->getEntryClass( iter->referenced, iter->modified ).erase( iter );

    iter->actual = true;
    iter->referenced = true;
    iter->modified = false;

    EntryClass & ec = group->getEntryClass( iter->referenced, iter->modified );
    ec.insert( iter );

    iter->tag = tag;
    iter->value = value;
}
