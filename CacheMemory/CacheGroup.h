#ifndef CACHEGROUP_H_INCLUDED
#define CACHEGROUP_H_INCLUDED

#include <vector>
#include <set>
#ifdef __DEBUG__
    #include <iostream>
#endif

class CacheGroup
{

private:

    class Entry;

    typedef std::vector<Entry> EntryVector;
    typedef std::set<EntryVector::iterator> EntryClass;

public:

    // typedefs
    typedef size_t index_t;
    typedef size_t tag_t;
    typedef char value_t;

    class Iterator
    {
    public:
        Iterator( CacheGroup *, EntryVector::iterator );

        bool isActual() const;
        bool isReferenced() const;
        bool isModified() const;

        value_t getValue();
        void setValue( value_t value );

        void setTagAndValue( tag_t tag, value_t value );

    private:

        CacheGroup * group;
        EntryVector::iterator iter;
    };

    // constructor
    CacheGroup( size_t size, size_t maxCount );

    // asseccors

    Iterator & operator[]( index_t );

    bool isActual( index_t ) const;
    void setActual( index_t, bool );

    bool isReferenced( index_t ) const;
    void setReferenced( index_t, bool );

    bool isModified( index_t ) const;
    void setModified( index_t, bool );

    tag_t getTag( index_t ) const;
    void setTag( index_t, tag_t );

    value_t getValue( index_t ) const;
    void setValue( index_t, value_t );

    size_t size() const;

    // find entry
    // if "cache hit" then return corresponding entry
    // if "cache miss" then return appropriete entry for replacing
    index_t callEntry( tag_t, bool & hit );
    Iterator callEntryIterator( tag_t, bool & hit );

private:

    class Entry
    {
    public:

        bool actual, referenced, modified;
        tag_t tag;
        value_t value;
    };

    inline EntryClass & getEntryClass( bool referenced, bool modified );
    EntryVector::iterator findEntry( tag_t tag );
    EntryVector::iterator findFree();
    EntryVector::iterator findNRU();
    void resetFlags();

    static void initEntry( Entry & );
    static bool cmpEntryTag( Entry e, tag_t t );
    static bool entryIsNotActual( const Entry & e );
    static EntryVector::iterator getRandomElem( EntryClass & ec );

    EntryVector entries;
    EntryClass classSets[ 4 ];

    size_t COUNT_MAX;
    size_t count;
};

#ifdef __DEBUG__
std::ostream & operator<<( std::ostream & ost, const CacheGroup & cg );
#endif

#endif // CACHEGROUP_H_INCLUDED
