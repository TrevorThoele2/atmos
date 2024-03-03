
#include <functional>

#include "ContextMenu.h"

namespace agui
{
    void ContextMenu::Connection::Destroy_impl()
    {
        obj.Destroy();
    }

    ContextMenu::Connection::Connection(const Menu::Connection &connection, Func func) : func(func)
    {
        obj = connection;
    }

    void ContextMenu::AddConnectionsToCursor()
    {
        if(cursor && menu)
        {
            for(auto loop = connections.begin(); loop != connections.end(); loop++)
                cursor->AddEntry(loop->obj.GetText());
        }
    }

    ContextMenu::Connections::iterator ContextMenu::FindConnection(Connections::ID index)
    {
        return connections.Find(index - 1);
    }

    ContextMenu::ContextMenu(Object *parent, const std::string &name, const RelativePosition &pos, float z) : Widget(parent, name, pos, z)
    {}

    ContextMenu::ContextMenu(Object *parent, const WidgetPrototype &arg) : Widget(parent, arg)
    {}

    ContextMenu::ContextMenu(Object *parent, const ContextMenu &arg) : Widget(parent, arg)
    {}

    void ContextMenu::RegisterInput(const Text &add, const RelativePosition &pos, Func func)
    {
        connections.AddBack(Connection(menu->CreateText(add.GetString(), 1, add), func))->obj.GetText().SetPosition(pos);
    }

    void ContextMenu::SetMenu(Menu &set)
    {
        menu.Set(*this, set);
        RegisterInputs();
        AddConnectionsToCursor();

        auto fitter = CreateInternalArranger<FitterRelationship>(true);
        fitter->SetSource(*menu.Get());
        fitter->SetTarget(*this);
    }

    Menu* ContextMenu::GetMenu()
    {
        return menu.Get();
    }

    void ContextMenu::SetCursor(Cursor &set)
    {
        cursor.Set(*this, set);
        AddConnectionsToCursor();
    }

    ContextMenu::Cursor* ContextMenu::GetCursor()
    {
        return cursor.Get();
    }

    void ContextMenu::OnActionPressed(const Atmos::Input::Action &args)
    {
        if(!IsActive())
            return;

        switch(args.id)
        {
        case Atmos::Input::ActionID::USE:
            FindConnection(cursor->GetCurrentEntry())->func();
            Hide();
            cursor->SetCurrentEntry(0);
            eventFinished();
            break;
        }
    }
}