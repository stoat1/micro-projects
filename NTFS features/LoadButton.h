#ifndef LOADBUTTON_H_INCLUDED
#define LOADBUTTON_H_INCLUDED

#include "Button.h"
#include "dialogs.h"

// button which reset localization to default
class LoadButton : public Button
{
public:

    LoadButton(MainWindow * owner)
    : Button(owner, "Load from file")
    {
        // move button
        move(2 * BUTTON_WIDTH + 3 * 10, 10, BUTTON_WIDTH, BUTTON_HEIGHT);
    }

    virtual void clicked()
    {
        // request user for filename to load
        pair<bool, string> fileName = getOpenFileName(hMainWindow);

        if(fileName.first)
        {
            getOwner()->getContext().loadFromFile(fileName.second);
        }
    }
};

#endif // LOADBUTTON_H_INCLUDED
