#include "MainWindow.h"
#include "ResetButton.h"
#include "SaveButton.h"
#include "LoadButton.h"
#include "LangList.h"
#include "ContentPane.h"

char MainWindow::className[] = "CodeBlocksWindowsApp";
map<HWND, MainWindow *> MainWindow::instances;

MainWindow::MainWindow(HINSTANCE hInstance) throw(runtime_error)
: hInstance(hInstance)
, hwnd(NULL)
, context()
, messageObservers()
, commandObservers()
, winDestr()
, resetButton(NULL)
, loadButton(NULL)
, saveButton(NULL)
, listBox(NULL)
, contentPane(NULL)
{
    registerWindowClass(hInstance);

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
        0,                   /* Extended possibilites for variation */
        className,         /* Classname */
        "Code::Blocks Template Windows App",       /* Title Text */
        WS_OVERLAPPEDWINDOW, /* default window */
        CW_USEDEFAULT,       /* Windows decides the position */
        CW_USEDEFAULT,       /* where the window ends up on the screen */
        544,                 /* The programs width */
        375,                 /* and height in pixels */
        HWND_DESKTOP,        /* The window is a child-window to desktop */
        NULL,                /* No menu */
        hInstance,       /* Program Instance handler */
        this                 /* No Window Creation data */
    );

    if(NULL == hwnd)
    {
        ostringstream osstr;
        osstr << "CreateWindowEx() returned NULL, error code is " << GetLastError();
        throw runtime_error(osstr.str());
    }

    if(0 != instances.count(hwnd))
    {
        throw runtime_error("CreateWindowEx() returned already existant hwnd");
    }

    instances.insert(make_pair(hwnd, this));
}

// destructor
MainWindow::~MainWindow()
{
    instances.erase(hwnd);
}

void MainWindow::addMessageObserver(UINT message, WindowObserver * observer)
{
    messageObservers[message].insert(observer);
}

void MainWindow::addCommandObserver(HMENU id, WindowObserver * observer)
{
    commandObservers[id].insert(observer);
}

void MainWindow::removeMessageObserver(UINT message, WindowObserver * observer)
{
    messageObservers[message].erase(observer);
}

void MainWindow::removeCommandObserver(HMENU id, WindowObserver * observer)
{
    commandObservers[id].erase(observer);
}

void MainWindow::show(int nCmdShow)
{
    ShowWindow(hwnd, nCmdShow);
}

HWND MainWindow::getHwnd()
{
    return hwnd;
}

HINSTANCE MainWindow::getHinstance()
{
    return hInstance;
}

Context & MainWindow::getContext()
{
    return context;
}

void MainWindow::displayContent(const string & streamName)
{
    cerr << "In displayContent(" << streamName << ")" << endl;
    Config * config = context.getLocal(streamName);
    contentPane->clear();
    if(config != NULL)
    {
        contentPane->clear(* config);
    }
    else
    {
        cerr << "error: config == NULL" << endl;
    }
}

void MainWindow::clearContent()
{
    cerr << "In clearContent()" << endl;
    contentPane->clear();
}

bool MainWindow::nofityObservers(const WindowEvent & event)
{
    if(WM_COMMAND == event.message)
    {
        HMENU id = reinterpret_cast<HMENU>(LOWORD(event.wParam));

        if(0 != commandObservers.count(id))
        {
            set<WindowObserver *> & observers = commandObservers[id];

            for(set<WindowObserver *>::iterator iObserver = observers.begin();
                iObserver != observers.end(); ++iObserver)
            {
                (* iObserver)->eventCame(event);
            }

            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        if(0 != messageObservers.count(event.message))
        {
            set<WindowObserver *> & observers = messageObservers[event.message];

            for(set<WindowObserver *>::iterator iObserver = observers.begin();
                iObserver != observers.end(); ++iObserver)
            {
                (* iObserver)->eventCame(event);
            }

            return true;
        }
        else
        {
            return false;
        }
    }
}

void MainWindow::initialize()
{
    cerr << "In MainWindow::initialize()" << endl;
    resetButton = new ResetButton(this);
    saveButton = new SaveButton(this);
    loadButton = new LoadButton(this);
    listBox = new LangList(this);
    contentPane = new ContentPane(this);

    // add observers
    addMessageObserver(WM_DESTROY, & winDestr);
}

void MainWindow::registerWindowClass(HINSTANCE hInstance) throw(runtime_error)
{
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hInstance;
    wincl.lpszClassName = className;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
    {
        ostringstream osstr;
        osstr << "RegisterClassEx() fails, error code is " << GetLastError();
        throw runtime_error(osstr.str());
    }
}

/*  This function is called by the Windows function DispatchMessage()  */
LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    bool processed = false;
    WindowEvent event(hwnd, message, wParam, lParam);

    // handle the messages
    try
    {
        if(WM_CREATE == message)
        {
            CREATESTRUCT * createStruct = reinterpret_cast<CREATESTRUCT *>(lParam);
            MainWindow * window = static_cast<MainWindow *>(createStruct->lpCreateParams);

            window->hwnd = hwnd;
            window->initialize();

            processed = true;
        }
//        else if(0 == MainWindow::instances.count(hwnd))
//        {
//            cerr << "WindowProcedure() called with unknown hwnd=" << hwnd << endl;
//            cerr << "\tmessage=" << hex << message << ", wParam=" << wParam << endl;
//        }
//        else
//        {
//            MainWindow * window = MainWindow::instances[hwnd];
//
//            if(WM_COMMAND == message)
//            {
//                HMENU id = reinterpret_cast<HMENU>(LOWORD(wParam));
//                map<HMENU, set<WindowObserver *> > & cmdObs = window->commandObservers;
//
//                if(cmdObs.find(id) != cmdObs.end())
//                {
//                    set<WindowObserver *> & obsVector = cmdObs[id];
//
//                    for(set<WindowObserver *>::iterator iObserver = obsVector.begin();
//                        iObserver != obsVector.end(); ++iObserver)
//                    {
//                        (* iObserver)->eventCame(event);
//                        processed = true;
//                    }
//                }
//            }
//
//            map<UINT, set<WindowObserver *> > & msgObs = window->messageObservers;
//
//            if(msgObs.find(message) != msgObs.end())
//            {
//                set<WindowObserver *> & obsVector = msgObs[message];
//
//                for(set<WindowObserver *>::iterator iObserver = obsVector.begin();
//                    iObserver != obsVector.end(); ++iObserver)
//                {
//                    (* iObserver)->eventCame(event);
//                    processed = true;
//                }
//            }
//        }

        else
        {
            if(0 != MainWindow::instances.count(hwnd))
            {
                MainWindow * window = MainWindow::instances[hwnd];
                processed = window->nofityObservers(event);

                if(! processed)
                {
                    return DefWindowProc(hwnd, message, wParam, lParam);
                }
            }
        }

        if(! processed)
        {
            return DefWindowProc(hwnd, message, wParam, lParam);
        }

    }
    catch(runtime_error & ex)
    {
        cout << "runtime_error: " << ex.what() << endl;
    }

    return 0;
}
