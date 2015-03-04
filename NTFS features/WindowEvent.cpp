#include "WindowEvent.h"



ostream & operator<<(ostream & ost, const WindowEvent & event)
{
    ost << "WindowEvent[";
    ost << "hwnd=" << event.hwnd << ", ";
    ost << "message=" << event.message << ", ";
    ost << "wParam=" << event.wParam << ", ";
    ost << "lParam=" << event.lParam << "]";
    return ost;
}
