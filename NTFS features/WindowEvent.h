#ifndef WINDOWEVENT_H_INCLUDED
#define WINDOWEVENT_H_INCLUDED

#include <windows.h>
#include <ostream>

using namespace std;

class WindowEvent
{
public:

    WindowEvent(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
    : hwnd(hwnd)
    , message(msg)
    , wParam(w)
    , lParam(l)
    {}

    HWND hwnd;
    UINT message;
    WPARAM wParam;
    LPARAM lParam;
};

ostream & operator<<(ostream & ost, const WindowEvent & event);

#endif // WINDOWEVENT_H_INCLUDED
