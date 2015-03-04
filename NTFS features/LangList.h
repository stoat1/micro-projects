#ifndef LANGLIST_H_INCLUDED
#define LANGLIST_H_INCLUDED

#include "ListBox.h"

class LangList : public ListBox
{
public:

    LangList(MainWindow * owner)
    : ListBox(owner)
    , updater(this)
    {
        getOwner()->getContext().addObserver(& updater);
    }

    virtual ~LangList()
    {}

    virtual void itemSelected(int index)
    {
        string item = getItem(index);

        getOwner()->clearContent();
        getOwner()->displayContent(item);
    }

    virtual void selectionCanceled()
    {
        getOwner()->clearContent();
    }

private:

    class LangListUpdater : public ContextObserver
    {
    public:

        LangListUpdater(LangList * langList)
        : langList(langList)
        {}

        virtual ~LangListUpdater()
        {}

        virtual void contentChanged()
        {
            // remove all items from listbox
            langList->clearItems();

            // notify main window that selection is canceled
            langList->selectionCanceled();

            // add items to listbox

            map<string, Context::Localization> & locals = langList->getOwner()->getContext().getContent();

            for(map<string, Context::Localization>::iterator iter = locals.begin();
                iter != locals.end(); ++iter)
            {
                langList->addItem(iter->first);
            }
        }

    private:

        LangListUpdater(const LangListUpdater & orig);
        LangListUpdater & operator=(const LangListUpdater & orig);

        LangList * langList;
    };

    LangListUpdater updater;
};

#endif // LANGLIST_H_INCLUDED
