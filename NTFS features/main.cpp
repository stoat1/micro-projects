#include <windows.h>
#include <commctrl.h>
#include <vector>
#include <iostream>
#include <cassert>
#include <sstream>
#include <cmath>
#include <set>

#include "dialogs.h"
#include "util.h"
#include "Config.h"
#include "MainWindow.h"
#include "Context.h"
#include "Button.h"
#include "Command.h"
#include "WindowInitializer.h"
#include "version.h"

using namespace std;

HWND hMainWindow;

class PrintVersion
{};

ostream & operator<<(ostream & ost, PrintVersion)
{
    using namespace AutoVersion;
    using AutoVersion::DATE;

    ost << "NTFS features " << FULLVERSION_STRING
        << " " << YEAR << "-" << MONTH << "-" << DATE;
    return ost;
}

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
char szClassName[] = "CodeBlocksWindowsApp";

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE /* hPrevInstance */,
                    LPSTR /* lpszArgument */,
                    int nCmdShow)
{
    cout << PrintVersion() << endl;

    /* Here messages to the application are saved */
    MSG messages;

    MainWindow mainWindow(hThisInstance);
    assert(hThisInstance == mainWindow.getHinstance());

    WindowObserver * initializer = new WindowInitializer(& mainWindow);
    mainWindow.addMessageObserver(WM_CREATE, initializer);

    /* Make the window visible on the screen */
    mainWindow.show(nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    delete initializer;

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}
