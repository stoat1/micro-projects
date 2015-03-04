#include "ListBox.h"
#include "SerialGenerator.h"

ListBox::ListBox(MainWindow * owner)
: hwnd(NULL)
, owner(owner)
, listObserver(this)
{
    RECT clientRect;
    GetClientRect(owner->getHwnd(), & clientRect);
    unsigned int windowHeight = clientRect.bottom - clientRect.top;

    SerialGenerator & gen = SerialGenerator::getInstance();
    HMENU id = gen();

    hwnd = CreateWindow(
        "listbox",
        "",
        WS_CHILD | WS_VISIBLE | LBS_STANDARD | LBS_WANTKEYBOARDINPUT,
        H_INDENT, 2 * V_INDENT + BUTTON_HEIGHT,
        LISTBOX_WIDTH, windowHeight - 3 * V_INDENT - BUTTON_HEIGHT,
        owner->getHwnd(),
        id,
        owner->getHinstance(),
        NULL
    );

    // observe parent window
    owner->addCommandObserver(id, & listObserver);
}

ListBox::~ListBox()
{}

size_t ListBox::getItemCount()
{
    return SendMessage(hwnd, LB_GETCOUNT, 0, 0L);
}

size_t ListBox::getSelectedIndex()
{
    return SendMessage(hwnd, LB_GETCURSEL, 0, 0L);
}

string ListBox::getItem(int index)
{
    static char buffer[1000];
    size_t len = SendMessage(hwnd, LB_GETTEXT, (WPARAM)index, (LPARAM)buffer);
    buffer[len] = '\0';
    return buffer;
}

void ListBox::addItem(const string & itemText)
{
    SendMessage(hwnd, LB_ADDSTRING, 0, (LPARAM)itemText.c_str());
}

void ListBox::clearItems()
{
    SendMessage(hwnd, LB_RESETCONTENT, 0, 0L);
}

// move button
void ListBox::move(int x, int y, unsigned int width, unsigned int height)
{
    MoveWindow(hwnd, x, y, width, height, TRUE);
}

// access to hwnd
HWND ListBox::getHwnd()
{
    return hwnd;
}

// access to owner
MainWindow * ListBox::getOwner()
{
    return owner;
}
