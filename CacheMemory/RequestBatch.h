#ifndef REQUESTBATCH_H_INCLUDED
#define REQUESTBATCH_H_INCLUDED

class RequestBatch
{
    class Executer;

public:

    RequestBatch()
    : totalRequestsSize( 0 ), reqs()
    {}

    addRequest( const MemRequest & req )
    {
        reqs.push_back( req );
        totalReqsSize += req.size();
    }

    void execute( AbstractMemory & storage )
    {
        for_each( reqs.begin(), reqs.end(), Executer( storage ) );
    }

    size_t size() const
    {
        return totalReqsSize;
    }

private:

    class Executer
    {
    public:

        Executer( AbstractMemory & storage )
        : storage( storage )
        {}

        void operator()( MemRequest & req )
        {
            req.execute( storage );
        }

    private:

        AbstractMemory & storage;
    }

    size_t totalReqsSize;
    std::vector<MemRequest> reqs;
};

#endif // REQUESTBATCH_H_INCLUDED
