#ifndef CATEGORICALREQUESTS_H_INCLUDED
#define CATEGORICALREQUESTS_H_INCLUDED

#include "RequestGenerator.h"
#include "UniformDistribution.h"
#include <vector>
#include <deque>
#include <set>

class CategoricalRequests : public RequestGenerator
{
public:

    typedef MemRequest::address_t address_t;

    CategoricalRequests( size_t storageSize, size_t catSize, double reReqProb )
        throw( std::invalid_argument );
    CategoricalRequests( const CategoricalRequests & orig );
    CategoricalRequests & operator=( const CategoricalRequests & orig );
    virtual ~CategoricalRequests();

    virtual MemRequest nextRequest();

private:

    struct Category
    {
    public:

        Category( address_t begin, address_t end )
        : begin( begin )
        , end( end )
        {}

        address_t begin, end;
    };

    size_t generateCatSize();
    double generateCatProb();

    Category & selectCategory();

    size_t storageSize;
    std::vector<Category> cats;
    std::deque<Category> recentCats;

    Random random;
    double reRequestProb;
    UniformDistribution * uniform;
};

#endif // CATEGORICALREQUESTS_H_INCLUDED
