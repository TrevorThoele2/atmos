
#pragma once

#include <AGUI/SpriteWidget.h>

#include "Environment.h"
#include "RandomAccessSequence.h"

namespace Agui
{
    class Cursor : public SpriteWidgetSync
    {
    public:
        typedef unsigned int Position;
    private:
        typedef Atmos::RandomAccessSequence<Object*> Objects;
    public:
        class Connection : public ::Agui::OwnerConnection<typename Objects::iterator, Cursor>
        {
        private:
            void Destroy_impl() override;
        public:
            Connection() = default;
            Connection(typename Objects::iterator itr, Cursor &owner);
            friend Cursor;
        };
    private:
        Position pos;
        Objects objects;

        void Setup();
        void ParentThisToSelected();
        void ChangePosTo(Position set);
        void ChangePos(bool add);
        void PosChanged(Position prev);
        void OnActionPressed(const ::Atmos::Input::Action &args);
        void OnNavMenuUp();
        void OnNavMenuDown();

        void RemoveEntry(typename Objects::iterator itr);
        typename Objects::iterator FindEntry(typename Objects::ID index);
    protected:
        Cursor(Object *parent, const std::string &name, const RelativePosition &pos, float z);
        Cursor(Object *parent, const WidgetPrototype &arg);
        Cursor(Object *parent, const Cursor &arg);
    public:
        NullEvent eventMoved;
        static Cursor* Factory(Object *parent, const std::string &name, const RelativePosition &pos, float z);
        static Cursor* Factory(Object *parent, const WidgetPrototype &arg);
        static Cursor* Factory(Object *parent, const Cursor &arg);
        Connection AddEntry(Object &add);
        void RemoveEntry(Connection &remove);
        void RemoveCurrentEntry();
        // Does not destroy the entries
        void ClearEntries();
        void SetCurrentEntry(Position set);
        void IncrementCurrentEntry(Position inc = 1);
        void DecrementCurrentEntry(Position dec = 1);
        void ResetCurrentEntry();
        Position GetCurrentEntry() const;
        WIDGET_COMMON_ACCESS(Cursor);
    };

	template<>
    class WidgetSetup<Cursor> : public WidgetSetupBase<Cursor>
    {
    private:
        void Setup(WidgetType &obj) const override
        {
            SetupOptional(obj, &WidgetType::CreateSprite<const SpriteComponent&>, sprite);
            SetupOptional(obj, &WidgetType::CreateSpriteFitter<>, spriteFitter);
        }
    public:
        ComponentAttribute<SpriteComponent> sprite;
        bool spriteFitter;

        WidgetSetup() = default;
    };

    using CursorSetup = WidgetSetup<Cursor>;
}