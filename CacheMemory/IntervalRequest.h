#ifndef INTERVALREQUEST_H_INCLUDED
#define INTERVALREQUEST_H_INCLUDED

#include "Request.h"
#include "Interval.h"

class IntervalRequest : public Request
{
public:
    typedef Request::Operation Operation;

    IntervalRequest( Interval anInterval, Operation anOperation );
    ~IntervalRequest();

    void execute( AbstractMemory & storage );

private:
    Interval interval;
    Operation operation;
};

#endif // INTERVALREQUEST_H_INCLUDED
