#ifndef BUTTONENABLER_H_INCLUDED
#define BUTTONENABLER_H_INCLUDED

#include <windows.h>

class ButtonEnabler : public AppObserver
{
public:

    ButtonEnabler(const ButtonEnabler & orig)
    : hOpenButton(orig.hOpenButton)
    , hSaveButton(orig.hSaveButton)
    , hSaveAsButton(orig.hSaveAsButton)
    , hCloseButton(orig.hCloseButton)
    {}

    ButtonEnabler & operator=(const ButtonEnabler & orig)
    {
        hOpenButton = orig.hOpenButton;
        hSaveButton = orig.hSaveButton;
        hSaveAsButton = orig.hSaveAsButton;
        hCloseButton = orig.hCloseButton;

        return * this;
    }

    ButtonEnabler(HWND hOpen, HWND hSave, HWND hSaveAs, HWND hClose)
    : hOpenButton(hOpen)
    , hSaveButton(hSave)
    , hSaveAsButton(hSaveAs)
    , hCloseButton(hClose)
    {}

    virtual ~ButtonEnabler() {}

    virtual void statusChanged(Application * source)
    {
        assert(NULL != source);

        EnableWindow(hOpenButton, (source->isFileOpened() ? FALSE : TRUE));
        EnableWindow(hSaveButton, (source->isFileSaved() ? FALSE : TRUE));
        EnableWindow(hSaveAsButton, (source->isFileSaved() ? FALSE : TRUE));
        EnableWindow(hCloseButton, (source->isFileOpened() ? TRUE : FALSE));
    }

    virtual void fileNameChanged(Application *)
    {

    }

private:

    HWND hOpenButton;
    HWND hSaveButton;
    HWND hSaveAsButton;
    HWND hCloseButton;
};

#endif // BUTTONENABLER_H_INCLUDED
