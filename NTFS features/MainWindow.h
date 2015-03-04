#ifndef WINDOWOBSERVABLE_H_INCLUDED
#define WINDOWOBSERVABLE_H_INCLUDED

#include <vector>
#include <map>
#include <set>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "WindowObserver.h"
#include "WindowDestructor.h"
#include "Context.h"

class ResetButton;
class LoadButton;
class SaveButton;
class ListBox;
class ContentPane;

using namespace std;

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

class MainWindow
{
public:

    // constructor
    MainWindow(HINSTANCE hInstance) throw(runtime_error);

    // destructor
    virtual ~MainWindow();

    // observers
    void addMessageObserver(UINT message, WindowObserver * observer);
    void addCommandObserver(HMENU id, WindowObserver * observer);
    void removeMessageObserver(UINT message, WindowObserver * obesrver);
    void removeCommandObserver(HMENU id, WindowObserver * observer);

    // show window
    void show(int nCmdShow);

    // access to winapi HWND
    HWND getHwnd();
    HINSTANCE getHinstance();

    // access to context
    Context & getContext();

    void displayContent(const string & streamName);
    void clearContent();

    // friend window function
    friend LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

private:

    // private copy constcutor and assignment operator
    MainWindow(const MainWindow & orig);
    MainWindow & operator=(const MainWindow & orig);

    bool nofityObservers(const WindowEvent & event);

    void initialize();

    static void registerWindowClass(HINSTANCE hInstance) throw(runtime_error);

    // handles
    HINSTANCE hInstance;
    HWND hwnd;

    // model
    Context context;

    // observers
    map<UINT, set<WindowObserver *> > messageObservers;
    map<HMENU, set<WindowObserver *> > commandObservers;

    WindowDestructor winDestr;

    // widgets
    ResetButton * resetButton;
    LoadButton * loadButton;
    SaveButton * saveButton;
    ListBox * listBox;
    ContentPane * contentPane;

    static char className[];

    static map<HWND, MainWindow *> instances;
};

#endif // WINDOWOBSERVABLE_H_INCLUDED
