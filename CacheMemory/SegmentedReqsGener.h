#ifndef SEGMENTEDREQSGENER_H_INCLUDED
#define SEGMENTEDREQSGENER_H_INCLUDED

#include "RequestGenerator.h"
#include "Distribution.h"
#include "Random.h"
#include <stdexcept>
#include <vector>

class SegmentedReqsGener : public RequestGenerator
{
public:

    SegmentedReqsGener( size_t storageSize, Distribution * segSizes,
        Distribution * addresses, Random rnd ) throw( std::invalid_argument );

    virtual ~SegmentedReqsGener();

    virtual MemRequest nextRequest();

private:

    SegmentedReqsGener( const SegmentedReqsGener & orig );
    void operator=( const SegmentedReqsGener & orig );

    typedef MemRequest::address_t address_t;

    size_t generateSegmentSize();
    address_t generateOffset();

    size_t storageSize;
    std::vector<size_t> segments;
    Distribution * segSizes, * addresses;
    Random random;
};


#endif // SEGMENTEDREQSGENER_H_INCLUDED
