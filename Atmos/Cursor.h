
#pragma once

#include <AGUI/SpriteWidget.h>

#include "InputAction.h"
#include "RandomAccessSequence.h"

namespace Atmos { class ObjectManager; }

namespace Agui
{
    class Cursor : public SpriteWidgetSync
    {
    public:
        typedef int Position;
    public:
        NullEvent onMoved;
    private:
        typedef Atmos::RandomAccessSequence<Object*> ObjectList;
    public:
        class Connection : public ::Agui::OwnerConnection<typename ObjectList::iterator, Cursor>
        {
        public:
            Connection() = default;
            Connection(typename ObjectList::iterator itr, Cursor& owner);
        private:
            void Destroy_impl() override;
        private:
            friend Cursor;
        };
    public:
        static Cursor* Factory(Object* parent, const std::string& name, const RelativePosition& pos, float z, ::Atmos::ObjectManager& objectManager);
        static Cursor* Factory(Object* parent, const WidgetPrototype& arg);
        static Cursor* Factory(Object* parent, const Cursor& arg);

        Connection AddEntry(Object& add);
        void RemoveEntry(Connection& remove);
        void RemoveCurrentEntry();
        // Does not destroy the entries
        void ClearEntries();
        void SetCurrentEntry(Position set);
        void IncrementCurrentEntry(Position inc = 1);
        void DecrementCurrentEntry(Position dec = 1);
        void ResetCurrentEntry();
        Position GetCurrentEntry() const;

        WIDGET_COMMON_ACCESS(Cursor);
    protected:
        Cursor(Object* parent, const std::string& name, const RelativePosition& pos, float z, ::Atmos::ObjectManager& objectManager);
        Cursor(Object* parent, const WidgetPrototype& arg);
        Cursor(Object* parent, const Cursor& arg);
    private:
        Position position;
        ObjectList objectList;
    private:
        void Setup();
        void ParentThisToSelected();
        void ChangePositionTo(Position set);
        void ChangePosition(bool add);
        void PositionChanged(Position previous);
    private:
        void RemoveEntry(typename ObjectList::iterator itr);
        typename ObjectList::iterator FindEntry(typename ObjectList::ID index);
    private:
        void OnActionPressed(const ::Atmos::Input::Action& args);
        void OnNavigateMenuUp();
        void OnNavigateMenuDown();
    private:
        ::Atmos::ObjectManager* objectManager;
    };

	template<>
    class WidgetSetup<Cursor> : public WidgetSetupBase<Cursor>
    {
    public:
        ComponentAttribute<SpriteComponent> sprite;
        bool spriteFitter;
    public:
        WidgetSetup() = default;
    private:
        void Setup(WidgetType& obj) const override
        {
            SetupOptional(obj, &WidgetType::CreateSprite<const SpriteComponent&>, sprite);
            SetupOptional(obj, &WidgetType::CreateSpriteFitter<>, spriteFitter);
        }
    };

    using CursorSetup = WidgetSetup<Cursor>;
}