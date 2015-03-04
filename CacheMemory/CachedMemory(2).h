#ifndef CACHEDARRAY_H_INCLUDED
#define CACHEDARRAY_H_INCLUDED

#include "AbstractMemory.h"
#include "CacheGroup.h"

#include <vector>
#include <list>
#include <set>
#ifdef __DEBUG__
    #include <iostream>
#endif
#include "Logger.h"

// TODO: incapsulate entry managing (changing, search in class) ot class Gruop

class CachedMemory : public AbstractMemory
{
public:

    typedef AbstractMemory::address_t address_t;
    typedef AbstractMemory::value_t value_t;

    // constructor and descructor
    explicit CachedMemory( AbstractMemory & mainMem, size_t cacheSize, size_t groupWidth,
                            size_t flagResetFreq );
    ~CachedMemory();

    value_t get( address_t ) throw( std::range_error );
    void set( address_t, value_t ) throw( std::range_error );

    address_t size() const;

    #ifdef __DEBUG__
    friend
    std::ostream & operator<<( std::ostream & ost, const CachedMemory & cm );
    #endif

private:

    typedef CacheGroup Group;
    typedef std::vector<Group *> Cache;
    typedef Group::tag_t tag_t;
    typedef size_t group_index_t;

    // maps address to group
    group_index_t whichGroup( address_t ) const;

    // maps tag of address
    tag_t getTag( address_t ) const;

    // find entry which represend given address of main memory
    static Cache::iterator findEntry( Cache::iterator begin,
        Cache::iterator end, address_t tag );

    // find free entry in cache
    static Cache::iterator findFreeEntry( Cache::iterator begin,
                                          Cache::iterator end );

    // remove not recently used entry
    // and return iterator pointing to released cell
    static Cache::iterator removeEntry( Cache::iterator begin,
                                           Cache::iterator end );


    const size_t GROUP_WIDTH;
    const size_t GROUPS;
    const size_t GROUP_SIZE;

    // main memory
    AbstractMemory & mainMemory;

    // cache memory
    Cache cache;

    static Logger & log;
    static std::ostringstream osstr;
};

#ifdef __DEBUG__
std::ostream & operator<<( std::ostream & ost, const CachedMemory & cm );
#endif

#endif // CACHEDARRAY_H_INCLUDED
