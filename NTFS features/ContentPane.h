#ifndef CONTENTPANE_H_INCLUDED
#define CONTENTPANE_H_INCLUDED

#include "Widget.h"
#include "Config.h"
#include "ContextObserver.h"
#include "MainWindow.h"

#include <vector>

class ContentPane : public Widget
{
public:

    ContentPane(MainWindow * owner);

    virtual ~ContentPane();

    void clear();
    void clear(Config & config);
    void saveToConfig(Config & config);

private:

    class ClearingObserver : public ContextObserver
    {
    public:

        ClearingObserver(ContentPane * pane)
        : pane(pane)
        {}

        virtual ~ClearingObserver()
        {}

        virtual void contentChanged()
        {
            pane->clear();
        }

    private:

        ClearingObserver(const ClearingObserver & orig);
        ClearingObserver & operator=(const ClearingObserver & orig);

        ContentPane * pane;
    };

    ContentPane(const ContentPane &);
    ContentPane & operator=(const ContentPane &);

    MainWindow * owner;
    Config * config;
    vector<pair<HWND, HWND> > items;

    static const size_t LABEL_WIDTH = 128;
    static const size_t FIELD_WIDTH = 128;
    static const size_t LINE_HEIGHT = 16;
};

#endif // CONTENTPANE_H_INCLUDED
