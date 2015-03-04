#ifndef WINDOWINITIALIZER_H_INCLUDED
#define WINDOWINITIALIZER_H_INCLUDED

#include "ResetButton.h"
#include "SaveButton.h"
#include "LoadButton.h"
#include "ListBox.h"
#include "WindowDestructor.h"
#include "ContentPane.h"

class WindowInitializer : public WindowObserver
{
public:

    WindowInitializer(MainWindow * windowToInit)
    : window(windowToInit)
    , winDestr()
    {}

    virtual void eventCame(WindowEvent)
    {
//        // add widgets
//        static ResetButton resetButton(window);
//        static SaveButton saveButton(window);
//        static LoadButton loadButton(window);
//        static ListBox listBox(window);
//        static ContentPane contentPane(window);
//
//        // add observers
//        window->addMessageObserver(WM_DESTROY, & winDestr);
    }

private:

    WindowInitializer(const WindowInitializer & orig);
    WindowInitializer & operator=(const WindowInitializer & orig);

    MainWindow * window;
    WindowDestructor winDestr;
};

#endif // WINDOWINITIALIZER_H_INCLUDED
