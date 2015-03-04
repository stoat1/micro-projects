#ifndef SAVEBUTTON_H_INCLUDED
#define SAVEBUTTON_H_INCLUDED

#include "Button.h"
#include "dialogs.h"

extern HWND hMainWindow;

// button which save localization to file
class SaveButton : public Button
{
public:

    SaveButton(MainWindow * owner)
    : Button(owner, "Save to file")
    {
        // move button
        move(BUTTON_WIDTH + 2 * 10, 10, BUTTON_WIDTH, BUTTON_HEIGHT);
    }

    virtual void clicked()
    {
        // request user for filename to save
        pair<bool, string> fileName = getSaveFileName(hMainWindow);

        if(fileName.first)
        {
            getOwner()->clearContent();

            getOwner()->getContext().saveToFile(fileName.second);
        }
    }
};

#endif // SAVEBUTTON_H_INCLUDED
