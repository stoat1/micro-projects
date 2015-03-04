#ifndef WINDOWOBSERVER_H_INCLUDED
#define WINDOWOBSERVER_H_INCLUDED

#include "WindowEvent.h"

class WindowObserver
{
public:
    virtual ~WindowObserver() {}
    virtual void eventCame(WindowEvent event) = 0;
};

#endif // WINDOWOBSERVER_H_INCLUDED
