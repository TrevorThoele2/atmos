
#pragma once

#include "Cursor.h"
#include "InputAction.h"
#include <AGUI\Menu.h>

namespace Agui
{
    class ContextMenu : public Widget
    {
    private:
        typedef std::function<void()> Func;
    protected:
        class Connection : Agui::Connection<Menu::Connection>
        {
        private:
            Func func;
            void Destroy_impl() override;
        public:
            Connection() = default;
            Connection(const Menu::Connection &connection, Func func);
            friend ContextMenu;
        };
    private:
        WidgetComponent<Menu> menu;
        WidgetComponent<Cursor> cursor;

        typedef Atmos::RandomAccessSequence<Connection> Connections;
        Connections connections;
        virtual void RegisterInputs() = 0;
        void AddConnectionsToCursor();
        Connections::iterator FindConnection(Connections::ID index);
    protected:
        ContextMenu(Object *parent, const std::string &name, const RelativePosition &pos, float z);
        ContextMenu(Object *parent, const WidgetPrototype &arg);
        ContextMenu(Object *parent, const ContextMenu &arg);
        void RegisterInput(const Text &add, const RelativePosition &pos, Func func);
    public:
        NullEvent eventFinished;
        virtual ~ContextMenu() = 0 {};

        template<class... Args>
        void CreateMenu(Args && ...args);
        void SetMenu(Menu &set);
        Menu* GetMenu();
        template<class... Args>
        void CreateCursor(Args && ...args);
        void SetCursor(Cursor &set);
        Cursor* GetCursor();

        void OnActionPressed(const Atmos::Input::Action &args);
    };

    template<class... Args>
    void ContextMenu::CreateMenu(Args && ...args)
    {
        SetMenu(*Menu::Factory(this, args...));
    }

    template<class... Args>
    void ContextMenu::CreateCursor(Args && ...args)
    {
        SetCursor(*Cursor::Factory(this, args...));
    }
}