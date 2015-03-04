#ifndef RESETBUTTON_H_INCLUDED
#define RESETBUTTON_H_INCLUDED

#include "Button.h"

// button which reset localization to default
class ResetButton : public Button
{
public:

    ResetButton(MainWindow * owner)
    : Button(owner, "Reset to default")
    {
        // move button
        move(10, 10, BUTTON_WIDTH, BUTTON_HEIGHT);
    }

    virtual void clicked()
    {
        getOwner()->getContext().reset();
    }
};

#endif // RESETBUTTON_H_INCLUDED
