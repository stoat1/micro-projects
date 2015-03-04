#ifndef CONTEXTOBSERVER_H_INCLUDED
#define CONTEXTOBSERVER_H_INCLUDED

class ContextObserver
{
public:
    virtual ~ContextObserver() {}
    virtual void contentChanged() = 0;
};

#endif // CONTEXTOBSERVER_H_INCLUDED
