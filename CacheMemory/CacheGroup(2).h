#ifndef CACHEGROUP_H_INCLUDED
#define CACHEGROUP_H_INCLUDED

#define ASSOCIATIVE_SEARCH

#include <vector>
#include <set>
#ifdef __DEBUG__
#   include <iostream>
#endif
#ifdef ASSOCIATIVE_SEARCH
#   include "AssocSearch.h"
#endif
#include "Logger.h"

#include "AbstractMemory.h"

class CacheGroup
{
private:

    CacheGroup( const CacheGroup & orig );
    CacheGroup & operator=( const CacheGroup & orig );

    class Entry;

    typedef std::vector<Entry> EntryVector;
    typedef std::set<EntryVector::iterator> EntryClass;

public:

    // typedefs
    typedef size_t index_t;
    typedef size_t tag_t;
    typedef AbstractMemory::value_t value_t;

    class Iterator
    {
    public:
        Iterator( CacheGroup *, EntryVector::iterator );

        bool isActual() const;
        bool isReferenced() const;
        bool isModified() const;

        bool isDirty() const
        {
            return isActual() && isModified();
        }

        tag_t getTag() const;

        value_t getValue();
        void setValue( value_t value );

        void setTagAndValue( tag_t tag, value_t value );

    private:

        CacheGroup * group;
        EntryVector::iterator iter;
    };

    // constructor
    CacheGroup( size_t size, size_t maxCount );
//    CacheGroup( const CacheGroup & orig );

    // asseccors

    Iterator & operator[]( index_t );

    size_t size() const;

    // find entry
    // if "cache hit" then return corresponding entry
    // if "cache miss" then return appropriete entry for replacing
    index_t callEntry( tag_t, bool & hit );
    Iterator callEntryIterator( tag_t, bool & hit );

#   ifdef __DEBUG__
    friend std::ostream & operator<<( std::ostream & ost, const CacheGroup & cg );
    friend std::ostream & operator<<( std::ostream & ost, const Entry & e );
#   endif

private:

    class Entry
    {
    public:

        bool actual, referenced, modified;
        tag_t tag;
        value_t value;
    };

#   ifdef ASSOCIATIVE_SEARCH
    class FindEntryFunctor : public AssocSearch<EntryVector::iterator>::Predicate
    {
    public:

        FindEntryFunctor( tag_t t )
        : tag( t )
        {}

        bool operator()( const EntryVector::iterator::value_type & entry )
        {
            bool result = ( entry.actual && ( tag == entry.tag ) );
            Logger & log = Logger::getInstance( "FindEntryFunctor debug" );
            ostringstream osstr;

            osstr << "e=" << entry;
            osstr << " t=" << tag << endl;
            log.println( osstr );

            return result;
        }

    private:

        tag_t tag;
    };

    class IsFreeFunctor : public AssocSearch<EntryVector::iterator>::Predicate
    {
    public:
        bool operator()( const EntryVector::iterator::value_type & entry )
        {
            return ! entry.actual;
        }
    };
#   endif

    // return reference to class of entries which correspond to given
    // combination or "referenced" and "modified" flags
    inline EntryClass & getEntryClass( bool referenced, bool modified );

    // find entry whose tag is equals to given value
    EntryVector::iterator findEntry( tag_t tag );

    // find entry which is not used
    EntryVector::iterator findFree();

    // find entry which is to be replaced according to NRU algorithm
    EntryVector::iterator findNRU();

    // reset "referenced" flags in all entries
    void resetFlags();

    static void initEntry( Entry & );
    static bool cmpEntryTag( Entry e, tag_t t );
    static bool entryIsNotActual( const Entry & e );
    static EntryVector::iterator getRandomElem( EntryClass & ec );

    EntryVector entries;
    EntryClass classSets[ 4 ];

    size_t COUNT_MAX;
    size_t count;

#   ifdef ASSOCIATIVE_SEARCH
    AssocSearch<EntryVector::iterator> assocSearch;
#   endif

    static Logger & log;
    static std::ostringstream osstr;
};

#ifdef __DEBUG__
std::ostream & operator<<( std::ostream & ost, const CacheGroup & cg );
#endif

#endif // CACHEGROUP_H_INCLUDED
