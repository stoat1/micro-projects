#ifndef BUTTONOBSERVER_H_INCLUDED
#define BUTTONOBSERVER_H_INCLUDED

class ButtonObserver
{
public:

    virtual ~ButtonObserver() {}
    virtual void buttonClicked() = 0;
};

#endif // BUTTONOBSERVER_H_INCLUDED
