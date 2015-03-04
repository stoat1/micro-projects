#ifndef WINDOWDESTRUCTOR_H_INCLUDED
#define WINDOWDESTRUCTOR_H_INCLUDED

#include "WindowObserver.h"

class WindowDestructor : public WindowObserver
{
public:

    WindowDestructor()
    {}

    virtual void eventCame(WindowEvent)
    {
        PostQuitMessage(0);
    }
};

#endif // WINDOWDESTRUCTOR_H_INCLUDED
