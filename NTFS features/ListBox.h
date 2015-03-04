#ifndef LISTBOX_H_INCLUDED
#define LISTBOX_H_INCLUDED

#include <vector>

#include "Widget.h"
#include "MainWindow.h"

class ListBox : public Widget
{
public:

    ListBox(MainWindow * owner);

    virtual ~ListBox();

    // list actions
    virtual void itemSelected(int selected) = 0;
    virtual void selectionCanceled() = 0;

    // list management
    size_t getItemCount();
    size_t getSelectedIndex();
    string getItem(int item);
    void addItem(const string & itemText);
    void clearItems();

    // move button
    void move(int x, int y, unsigned int width, unsigned int height);

    // access to hwnd
    HWND getHwnd();

    // access to owner
    MainWindow * getOwner();

private:

    // copy constructor and assignment operator
    ListBox(const ListBox & orig);
    ListBox & operator=(const ListBox & orig);

    class ListObserver : public WindowObserver
    {
    public:

        // constructor
        ListObserver(ListBox * listBox)
        : listBox(listBox)
        {}

        virtual ~ListObserver()
        {}

        virtual void eventCame(WindowEvent event)
        {
            if(LBN_SELCHANGE == HIWORD(event.wParam))
            {
                int nSelectedItem = listBox->getSelectedIndex();
                listBox->itemSelected(nSelectedItem);
            }
            else if(LBN_SELCANCEL == HIWORD(event.wParam))
            {
                listBox->selectionCanceled();
            }
        }

    private:

        // copy constructor and assignment operator
        ListObserver(const ListObserver & orig);
        ListObserver & operator=(const ListObserver & orig);

        ListBox * listBox;
    };

    HWND hwnd;
    MainWindow * owner;
    ListObserver listObserver;
};

#endif // LISTBOX_H_INCLUDED
