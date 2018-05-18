
#pragma once

#include "ObjectMenuTraits.h"

#include "Environment.h"
#include "RandomAccessSequence.h"
#include "StringUtility.h"

#include <AGUI\Menu.h>
#include "Cursor.h"
#include "FontDefines.h"

namespace Atmos
{
    namespace Detail
    {
        template<class Itr>
        struct ObjectMenuTraits;
    }

    template<class Obj, class List, class DescriptionBox, class ContextMenu>
    class ObjectMenu
    {
    public:
        typedef Obj Obj;
        typedef List List;
        typedef DescriptionBox DescriptionBox;
        typedef ContextMenu ContextMenu;
        typedef agui::Menu WrappedMenuT;
    private:
        struct Entry
        {
            WrappedMenuT::Connection menuConnection;
            Obj &wrapped;
            Entry(const agui::Menu::Connection &menuConnection, Obj &wrapped) : menuConnection(menuConnection), wrapped(wrapped) {}
        };

        typedef agui::Cursor<unsigned short> Cursor;
        typedef ObjectMenuTraits<List> Traits;
    private:
        bool active;

        WrappedMenuT *menu;
        Cursor *cursor;
        DescriptionBox *description;
        ContextMenu *contextMenu;

        List *list;
        typedef RandomAccessSequence<Entry> Entries;
        typedef typename Entries::iterator iterator;
        Entries entries;
        iterator selectedEntry;

        ObjectMenu(const ObjectMenu &arg) = delete;
        void OnActionPressed(const Atmos::Input::Action &args);
        void OnCursorMoved();
        void OnContextMenuDone();
        void GotoContextMenu();

        void CreateEntries();
        void DestroyEntries();
        void EraseEntry(iterator itr);
        iterator FindEntry(typename Entries::ID index);
        void VoidSelected();
        void SelectEntry(iterator select);

        virtual void OnGotoContextMenu() {}
    protected:
        template<class T>
        void InitBase(T &root, DescriptionBox &description, ContextMenu *contextMenu);
        List* GetList();
    public:
        NullEvent eventUpdate;

        ObjectMenu();
        virtual ~ObjectMenu() = 0 {}
        void Start();
        void Pause();
        void Update();
        void Stop();
        bool IsActive() const;

        void SetList(List *list);

        Obj* GetSelected();
        WrappedMenuT* GetMenu();
        Cursor* GetCursor();
        DescriptionBox* GetDescriptionBox();
        ContextMenu* GetContextMenu();
        void SetDescriptionBox(DescriptionBox &set);
        void SetContextMenu(ContextMenu &set);
    };

    template<class Obj, class List, class DescriptionBox, class ContextMenu>
    void ObjectMenu<Obj, List, DescriptionBox, ContextMenu>::OnActionPressed(const Atmos::Input::Action &args)
    {
        if (!active)
            return;

        if (contextMenu && contextMenu->IsActive())
            contextMenu->OnActionPressed(args);
        else
        {
            switch (args.id)
            {
            case Input::ActionID::USE:
                if (contextMenu && !contextMenu->IsActive() && !entries.empty())
                    GotoContextMenu();

                break;
            }
        }
    }

    template<class Obj, class List, class DescriptionBox, class ContextMenu>
    void ObjectMenu<Obj, List, DescriptionBox, ContextMenu>::OnCursorMoved()
    {
        if (entries.size() > 0)
            SelectEntry(FindEntry(cursor->GetCurrentEntry()));
    }

    template<class Obj, class List, class DescriptionBox, class ContextMenu>
    void ObjectMenu<Obj, List, DescriptionBox, ContextMenu>::OnContextMenuDone()
    {
        if (!active)
            return;

        cursor->Activate();
        Update();
    }

    template<class Obj, class List, class DescriptionBox, class ContextMenu>
    void ObjectMenu<Obj, List, DescriptionBox, ContextMenu>::GotoContextMenu()
    {
        cursor->Deactivate();
        contextMenu->SetSelected(selectedEntry->wrapped);
        OnGotoContextMenu();
        contextMenu->Show();
    }

    template<class Obj, class List, class DescriptionBox, class ContextMenu>
    void ObjectMenu<Obj, List, DescriptionBox, ContextMenu>::CreateEntries()
    {
        typename Entries::size_type count = 0;
        auto &container = Traits::RetrieveContainer(*list);
        for (auto loop = container.begin(); loop != container.end(); ++loop)
        {
            auto &unpacked = Traits::Unpack(loop);
            auto &connection = menu->CreateText(ToString(count), 1, agui::Text(unpacked.GetName(), agui::Text::Format(), *agui::fontSlender, agui::Color(255, 255, 255, 255)));
            auto &emplaced = entries.AddBack(Entry(connection, unpacked));
            emplaced->menuConnection.GetText().SetAutoCalcTextSize();
            emplaced->menuConnection.GetText().color.Edit(0, 0, 0);
            cursor->AddEntry(emplaced->menuConnection.GetText());
            ++count;
        }

        SelectEntry(entries.begin());
    }

    template<class Obj, class List, class DescriptionBox, class ContextMenu>
    void ObjectMenu<Obj, List, DescriptionBox, ContextMenu>::DestroyEntries()
    {
        for (auto loop = entries.begin(); loop != entries.end(); loop++)
            loop->menuConnection.Destroy();

        entries.Clear();
        cursor->ClearEntries();
        VoidSelected();
    }

    template<class Obj, class List, class DescriptionBox, class ContextMenu>
    void ObjectMenu<Obj, List, DescriptionBox, ContextMenu>::EraseEntry(iterator itr)
    {
        if (entries.empty())
            return;

        if (selectedEntry == itr)
        {
            auto test = selectedEntry;
            auto next = std::next(test);
            auto prev = std::prev(test);
            if (next != entries.end())
                SelectEntry(next);
            else if (prev != entries.end())
                SelectEntry(prev);
            else VoidEntry();
        }

        itr->menuConnection.Destroy();
        entries.Remove(itr);
    }

    template<class Obj, class List, class DescriptionBox, class ContextMenu>
    typename ObjectMenu<Obj, List, DescriptionBox, ContextMenu>::iterator ObjectMenu<Obj, List, DescriptionBox, ContextMenu>::FindEntry(typename Entries::ID index)
    {
        return entries.Find(index - 1);
    }

    template<class Obj, class List, class DescriptionBox, class ContextMenu>
    void ObjectMenu<Obj, List, DescriptionBox, ContextMenu>::VoidSelected()
    {
        selectedEntry = entries.end();
        description->Set();
    }

    template<class Obj, class List, class DescriptionBox, class ContextMenu>
    void ObjectMenu<Obj, List, DescriptionBox, ContextMenu>::SelectEntry(iterator select)
    {
        if (select == entries.end())
        {
            VoidSelected();
            return;
        }

        selectedEntry = select;
        description->Set(selectedEntry->wrapped);
    }

    template<class Obj, class List, class DescriptionBox, class ContextMenu>
    template<class T>
    void ObjectMenu<Obj, List, DescriptionBox, ContextMenu>::InitBase(T &root, DescriptionBox &description, ContextMenu *contextMenu)
    {
        this->description = &description;
        this->contextMenu = contextMenu;

        Environment::GetInput()->eventActionPressed.Subscribe(&ObjectMenu::OnActionPressed, *this);

        // Menu
        menu = agui::Menu::Factory(&root, "menu", agui::RelativePosition(), 0);
        menu->GetLayout()->SetSelfModifySize(false);

        // Cursor
        cursor = Cursor::Factory(menu, "cursor", agui::RelativePosition(agui::Dimension(0, -20), agui::Dimension()), 0);
        cursor->SetActiveFollowsShow(false);
        cursor->Deactivate();
        cursor->eventMoved.Subscribe(std::bind(&ObjectMenu::OnCursorMoved, this));

        // Context menu
        if (this->contextMenu)
            this->contextMenu->eventFinished.Subscribe(std::bind(&ObjectMenu::OnContextMenuDone, this));
    }

    template<class Obj, class List, class DescriptionBox, class ContextMenu>
    typename ObjectMenu<Obj, List, DescriptionBox, ContextMenu>::List* ObjectMenu<Obj, List, DescriptionBox, ContextMenu>::GetList()
    {
        return list;
    }

    template<class Obj, class List, class DescriptionBox, class ContextMenu>
    ObjectMenu<Obj, List, DescriptionBox, ContextMenu>::ObjectMenu() : active(false), menu(nullptr), cursor(nullptr), description(nullptr), contextMenu(nullptr), list(nullptr), selectedEntry(entries.end())
    {}

    template<class Obj, class List, class DescriptionBox, class ContextMenu>
    void ObjectMenu<Obj, List, DescriptionBox, ContextMenu>::Start()
    {
        cursor->Activate();
        cursor->GetSprite()->color.alpha = 255;
        active = true;
    }

    template<class Obj, class List, class DescriptionBox, class ContextMenu>
    void ObjectMenu<Obj, List, DescriptionBox, ContextMenu>::Pause()
    {
        cursor->Deactivate();
        cursor->GetSprite()->color.alpha = 127;
        active = false;
    }

    template<class Obj, class List, class DescriptionBox, class ContextMenu>
    void ObjectMenu<Obj, List, DescriptionBox, ContextMenu>::Update()
    {
        DestroyEntries();
        CreateEntries();
        eventUpdate();
    }

    template<class Obj, class List, class DescriptionBox, class ContextMenu>
    void ObjectMenu<Obj, List, DescriptionBox, ContextMenu>::Stop()
    {
        DestroyEntries();
        VoidSelected();
        active = false;
    }

    template<class Obj, class List, class DescriptionBox, class ContextMenu>
    bool ObjectMenu<Obj, List, DescriptionBox, ContextMenu>::IsActive() const
    {
        return active;
    }

    template<class Obj, class List, class DescriptionBox, class ContextMenu>
    void ObjectMenu<Obj, List, DescriptionBox, ContextMenu>::SetList(List *list)
    {
        if (list)
        {
            auto prev = this->list;
            this->list = list;
            Update();
        }
        else
        {
            this->list = nullptr;
            Stop();
        }
    }

    template<class Obj, class List, class DescriptionBox, class ContextMenu>
    Obj* ObjectMenu<Obj, List, DescriptionBox, ContextMenu>::GetSelected()
    {
        auto found = entries.Find(cursor->GetCurrentEntry());
        if (found == entries.end())
            return nullptr;

        return &found->wrapped;
    }

    template<class Obj, class List, class DescriptionBox, class ContextMenu>
    typename ObjectMenu<Obj, List, DescriptionBox, ContextMenu>::WrappedMenuT* ObjectMenu<Obj, List, DescriptionBox, ContextMenu>::GetMenu()
    {
        return menu;
    }

    template<class Obj, class List, class DescriptionBox, class ContextMenu>
    typename ObjectMenu<Obj, List, DescriptionBox, ContextMenu>::Cursor* ObjectMenu<Obj, List, DescriptionBox, ContextMenu>::GetCursor()
    {
        return cursor;
    }

    template<class Obj, class List, class DescriptionBox, class ContextMenu>
    DescriptionBox* ObjectMenu<Obj, List, DescriptionBox, ContextMenu>::GetDescriptionBox()
    {
        return description;
    }

    template<class Obj, class List, class DescriptionBox, class ContextMenu>
    ContextMenu* ObjectMenu<Obj, List, DescriptionBox, ContextMenu>::GetContextMenu()
    {
        return contextMenu;
    }

    template<class Obj, class List, class DescriptionBox, class ContextMenu>
    void ObjectMenu<Obj, List, DescriptionBox, ContextMenu>::SetDescriptionBox(DescriptionBox &set)
    {
        description = &set;
    }

    template<class Obj, class List, class DescriptionBox, class ContextMenu>
    void ObjectMenu<Obj, List, DescriptionBox, ContextMenu>::SetContextMenu(ContextMenu &set)
    {
        contextMenu = &set;
    }
}