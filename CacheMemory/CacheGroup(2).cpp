#include "CacheGroup.h"
#include <cassert>
#include <algorithm>
#include <functional>
#include <stdexcept>
#include "Logger.h"

using namespace std;

CacheGroup::CacheGroup( size_t size, size_t maxCount )
: entries( size, Entry() ), COUNT_MAX( maxCount ), count( 0 )
#ifdef ASSOCIATIVE_SEARCH
, assocSearch( entries.begin(), entries.end(), size / 4 + 1 )
#endif
{
    osstr << "In CacheGroup::CacheGroup(size=" << size
        << ", maxCount=" << maxCount << ")";
    log.println( osstr );

    // check argument
    if( size <= 0 )
    {
        throw invalid_argument( "CacheGroup size must be a positive integer" );
    }

    for( EntryVector::iterator iter = entries.begin();
        iter != entries.end(); ++iter )
    {
        initEntry( * iter );

        getEntryClass( iter->referenced, iter->modified ).insert( iter );
    }

    assert( classSets[ 0 ].size() + classSets[ 1 ].size() + classSets[ 2 ].size()
        + classSets[ 3 ].size() == entries.size() );
}

//CacheGroup::CacheGroup( const CacheGroup & orig )
//: entries( orig.entries ), COUNT_MAX( orig.COUNT_MAX ), count( orig.count )
//{
//    classSets[ 0 ].clear();
//    classSets[ 1 ].clear();
//    classSets[ 2 ].clear();
//    classSets[ 3 ].clear();
//
//    for( EntryVector::iterator iter = entries.begin();
//        iter != entries.end(); ++iter )
//    {
//        initEntry( * iter );
//
//        getEntryClass( iter->referenced, iter->modified ).insert( iter );
//    }
//}

size_t CacheGroup::size() const
{
    return entries.size();
}

CacheGroup::Iterator CacheGroup::callEntryIterator( tag_t tag, bool & hit )
{
    osstr << "In CacheGroup::callEntryIterator(tag=" << tag << ")";
    log.println( osstr );

    hit = true;

    EntryVector::iterator found = findEntry( tag );
    if( found == entries.end() )
    {
        log.println( "miss" );

        // cache miss
        hit = false;

        // find appropriate entry for replacing
        found = findFree();
        if( found == entries.end() )
        {
            log.println( "no free entreis" );

            found = findNRU();
        }
    }
    else
    {
        log.println( "hit" );
    }

    osstr << "return entry #" << found - entries.begin();
    log.println( osstr );

    ++count;
    if( count > COUNT_MAX )
    {
        log.println( "reset all flags" );

        resetFlags();
        count = 0;
    }

    return Iterator( this, found );
}

CacheGroup::EntryClass & CacheGroup::getEntryClass( bool r, bool m )
{
    return classSets[ ( r ? 2 : 0 ) | ( m ? 1 : 0 ) ];
}

CacheGroup::EntryVector::iterator
    CacheGroup::findEntry( tag_t tag )
#ifdef ASSOCIATIVE_SEARCH
{
    FindEntryFunctor * pred = new FindEntryFunctor( tag );
    assocSearch.setCollection( entries.begin(), entries.end() );
    EntryVector::iterator found = assocSearch.findIf( pred );
    delete pred;
    return found;
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
#ifdef ASSOCIATIVE_SEARCH
{
    IsFreeFunctor pred;
    EntryVector::iterator found = assocSearch.findIf( & pred );
    return found;
}
#else
{
    EntryVector::iterator found =
        find_if( entries.begin(), entries.end(),
            ptr_fun( entryIsNotActual ) );
    return found;
}
#endif

CacheGroup::EntryVector::iterator
    CacheGroup::findNRU()
{
    log.println( "In findNRU()" );

    int i;

    for( i = 0; i < 4; ++i )
    {
        if( ! classSets[ i ].empty() )
            break;
    }
    assert( i != 4 );

    osstr << "class is " << i;
    log.println( osstr );

    EntryClass & ec = classSets[ i ];
    return getRandomElem( ec );
}

void CacheGroup::resetFlags()
{
    for( EntryVector::iterator iter = entries.begin();
        iter != entries.end(); ++iter )
    {
        getEntryClass( iter->referenced, iter->modified ).erase( iter );
        iter->referenced = false;
        getEntryClass( iter->referenced, iter->modified ).insert( iter );
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
    log.println( "In getRandomElem( ec )" );
    osstr << "ec.size() = " << ec.size();
    log.println( osstr );

    EntryClass::iterator it = ec.begin();
//    advance<EntryClass::iterator, size_t>( it, rand() % ec.size() );
    return * it;
}

#ifdef __DEBUG__
ostream & operator<<( ostream & ost, const CacheGroup::Entry & e )
{
    ost << "Entry (" << & e << ")[";
    ost << "a=" << e.actual;
    ost << " r=" << e.referenced;
    ost << " m=" << e.modified;
    ost << " tag=" << e.tag;
    ost << " value=" << e.value;
    ost << "]";
    return ost;
}

ostream & operator<<( ostream & ost, const CacheGroup & cg )
{
    ost << "CacheGroup [";

    ost << endl;
    for( size_t i = 0; i < cg.size(); ++i )
    {
        const CacheGroup::Entry & entry = cg.entries.at( i );

        ost << "\t" << entry << endl;
//        if( entry.actual )
//        {
//            ost << endl;
//            ost << "\t" << i << " : 0x" << hex << entry.tag << " "
//                << dec << entry.value;
//            if( entry.referenced )
//                ost << " r";
//            if( entry.modified )
//                ost << " m";
//        }
    }

    ost << endl;
    for( size_t i = 0; i < 4; ++i )
    {
        string className;

        if( 0 == i )
            className = "0";
        else if( 1 == i )
            className = "M";
        else if( 2 == i )
            className = "R";
        else
            className = "RM";

        ost << "\t" << "class " << className << ": ";
        for( CacheGroup::EntryClass::iterator iter = cg.classSets[ i ].begin();
            iter != cg.classSets[ i ].end(); ++iter )
        {
            ost << * iter - cg.entries.begin() << " ";
        }
        ost << endl;
    }

    ost << "]";

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

CacheGroup::tag_t CacheGroup::Iterator::getTag() const
{
    return iter->tag;
}

CacheGroup::value_t CacheGroup::Iterator::getValue()
{
    group->getEntryClass( iter->referenced, iter->modified ).erase( iter );
    iter->referenced = true;
    group->getEntryClass( iter->referenced, iter->modified ).insert( iter );

    return iter->value;
}

void CacheGroup::Iterator::setValue( value_t value )
{
    group->getEntryClass( iter->referenced, iter->modified ).erase( iter );
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

    group->getEntryClass( iter->referenced, iter->modified ).insert( iter );

    iter->tag = tag;
    iter->value = value;
}

Logger & CacheGroup::log = Logger::getInstance( "CacheGroup debug" );

ostringstream CacheGroup::osstr;
