#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

#include <windows.h>
#include <commctrl.h>

#include "Widget.h"
#include "SerialGenerator.h"
#include "Command.h"
#include "MainWindow.h"
#include "ButtonObserver.h"

class Button : public Widget
{
public:

    // constructor
    Button(MainWindow * owner, const string & text)
    : hwnd(NULL)
    , owner(owner)
    , clickInvoker(this)
    {
        // generator button id
        SerialGenerator & gen = SerialGenerator::getInstance();
        HMENU id = gen();

        // create button window
        hwnd = CreateWindow(
            "button",
            text.c_str(),
            WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
            0, 0,
            BUTTON_WIDTH, BUTTON_HEIGHT,
            owner->getHwnd(),
            id,
            owner->getHinstance(),
            NULL
        );

        // observe the window
        owner->addCommandObserver(id, & clickInvoker);
    }

    // destructor
    virtual ~Button()
    {}

    // action for click event
    virtual void clicked() = 0;

    // move button
    void move(int x, int y, unsigned int width, unsigned int height)
    {
        MoveWindow(hwnd, x, y, width, height, TRUE);
    }

    // access to hwnd
    HWND getHwnd()
    {
        return hwnd;
    }

    // access to owner
    MainWindow * getOwner()
    {
        return owner;
    }

private:

    // copy constructor and assignment operator
    Button(const Button & orig);
    Button & operator=(const Button & orig);

    class ClickInvoker : public WindowObserver
    {
    public:

        ClickInvoker(Button * button)
        : button(button)
        {}

        virtual void eventCame(WindowEvent)
        {
            button->clicked();
        }

    private:

        // copy constructor and assignment operator
        ClickInvoker(const ClickInvoker & orig);
        ClickInvoker & operator=(const ClickInvoker & orig);

        Button * button;
    };

    HWND hwnd;
    MainWindow * owner;
    ClickInvoker clickInvoker;
};

#endif // BUTTON_H_INCLUDED
