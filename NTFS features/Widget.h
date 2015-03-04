#ifndef WIDGET_H_INCLUDED
#define WIDGET_H_INCLUDED

#include <windows.h>

extern HINSTANCE hInstance;

class Widget
{
public:
    virtual ~Widget() {}

protected:
    static const unsigned int H_INDENT = 12;
    static const unsigned int V_INDENT = 12;
    static const unsigned int BUTTON_WIDTH = 128;
    static const unsigned int BUTTON_HEIGHT = 24;
    static const unsigned int LISTBOX_WIDTH = 256;
};

#endif // WIDGET_H_INCLUDED
