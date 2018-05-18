
#pragma once

#include <AGUI\SpriteWidget.h>

#include "Environment.h"
#include "RandomAccessSequence.h"

namespace agui
{
    template<class PositionT>
    class Cursor : public SpriteWidgetSync
    {
    public:
        typedef PositionT Position;
    private:
        typedef Atmos::RandomAccessSequence<Object*> Objects;
    public:
        class Connection : public agui::OwnerConnection<typename Objects::iterator, Cursor>
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
        void OnActionPressed(const Atmos::Input::Action &args);
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
        WIDGET_COMMON_ACCESS(Cursor<PositionT>);
    };

    template<class PositionT>
    void Cursor<PositionT>::Connection::Destroy_impl()
    {
        owner->RemoveEntry(*this);
    }

    template<class PositionT>
    Cursor<PositionT>::Connection::Connection(typename Objects::iterator itr, Cursor &owner) : OwnerConnection(itr, owner)
    {}

    template<class PositionT>
    void Cursor<PositionT>::Setup()
    {
        ::Atmos::Environment::GetInput()->eventActionPressed.Subscribe(&Cursor::OnActionPressed, *this);
        SetClippingMethod(ClippingMethod::DONT);
    }

    template<class PositionT>
    void Cursor<PositionT>::ParentThisToSelected()
    {
        auto object = *FindEntry(pos);
        object->AddChild(*this);
    }

    template<class PositionT>
    void Cursor<PositionT>::ChangePosTo(Position set)
    {
        auto previousPos = pos;
        pos = set;
        PosChanged(previousPos);
    }

    template<class PositionT>
    void Cursor<PositionT>::ChangePos(bool add)
    {
        auto previousPos = pos;
        const Position upperBound = static_cast<Position>(objects.size());
        const Position lowerBound = 1;
        (add) ? ++pos : --pos;
        if (pos > upperBound)
            pos = lowerBound;
        else if (pos < lowerBound)
            pos = upperBound;
        PosChanged(previousPos);
    }

    template<class PositionT>
    void Cursor<PositionT>::PosChanged(Position prev)
    {
        if(prev != pos && !objects.empty())
        {
            ParentThisToSelected();
            eventMoved();
        }
    }

    template<class PositionT>
    void Cursor<PositionT>::OnActionPressed(const Atmos::Input::Action &args)
    {
        if(!IsActive())
            return;

        if(args.id == Atmos::Input::ActionID::NAVIGATE_MENU_UP)
            OnNavMenuUp();
        else if(args.id == Atmos::Input::ActionID::NAVIGATE_MENU_DOWN)
            OnNavMenuDown();
    }

    template<class PositionT>
    void Cursor<PositionT>::OnNavMenuUp()
    {
        ChangePos(false);
    }

    template<class PositionT>
    void Cursor<PositionT>::OnNavMenuDown()
    {
        ChangePos(true);
    }

    template<class PositionT>
    void Cursor<PositionT>::RemoveEntry(typename Objects::iterator itr)
    {
        objects.Remove(itr);
        ParentThisToSelected();
    }

    template<class PositionT>
    Cursor<PositionT>::Objects::iterator Cursor<PositionT>::FindEntry(typename Objects::ID index)
    {
        return objects.Find(index - 1);
    }

    template<class PositionT>
    Cursor<PositionT>::Cursor(Object *parent, const std::string &name, const RelativePosition &relPos, float z) : SpriteWidgetSync(parent, name, relPos, z), pos(1)
    {
        Setup();
    }

    template<class PositionT>
    Cursor<PositionT>::Cursor(Object *parent, const WidgetPrototype &arg) : SpriteWidgetSync(parent, arg), pos(1)
    {
        Setup();
    }

    template<class PositionT>
    Cursor<PositionT>::Cursor(Object *parent, const Cursor &arg) : SpriteWidgetSync(parent, arg), pos(arg.pos)
    {
        Setup();
    }

    template<class PositionT>
    Cursor<PositionT>* Cursor<PositionT>::Factory(Object *parent, const std::string &name, const RelativePosition &pos, float z)
    {
        return CreateBase<Cursor>(parent, name, pos, z);
    }

    template<class PositionT>
    Cursor<PositionT>* Cursor<PositionT>::Factory(Object *parent, const WidgetPrototype &arg)
    {
        return CreateBase<Cursor>(parent, arg);
    }

    template<class PositionT>
    Cursor<PositionT>* Cursor<PositionT>::Factory(Object *parent, const Cursor &arg)
    {
        return CreateBase<Cursor>(parent, arg);
    }

    template<class PositionT>
    typename Cursor<PositionT>::Connection Cursor<PositionT>::AddEntry(Object &add)
    {
        Objects::iterator added = objects.end();
        if(objects.empty())
        {
            added = objects.AddBack(&add);
            (*added)->AddChild(*this);
        }
        else
            added = objects.AddBack(&add);

        return Connection(added, *this);
    }

    template<class PositionT>
    void Cursor<PositionT>::RemoveEntry(Connection &remove)
    {
        if(remove.owner != this || objects.empty())
            return;

        RemoveEntry(remove.obj);
    }

    template<class PositionT>
    void Cursor<PositionT>::RemoveCurrentEntry()
    {
        RemoveEntry(FindEntry(pos.Get()));
    }

    template<class PositionT>
    void Cursor<PositionT>::ClearEntries()
    {
        RemoveFromParent();
        objects.Clear();
    }

    template<class PositionT>
    void Cursor<PositionT>::SetCurrentEntry(Position set)
    {
        ChangePosTo(set);
    }

    template<class PositionT>
    void Cursor<PositionT>::IncrementCurrentEntry(Position inc)
    {
        SetCurrentEntry(GetCurrentEntry() + inc);
    }

    template<class PositionT>
    void Cursor<PositionT>::DecrementCurrentEntry(Position dec)
    {
        SetCurrentEntry(GetCurrentEntry() - dec);
    }

    template<class PositionT>
    void Cursor<PositionT>::ResetCurrentEntry()
    {
        ChangePosTo(1);
    }

    template<class PositionT>
    typename Cursor<PositionT>::Position Cursor<PositionT>::GetCurrentEntry() const
    {
        return pos;
    }

    template<class PositionT>
    class WidgetSetup<Cursor<PositionT>> : public WidgetSetupBase<Cursor<PositionT>>
    {
    private: typedef Cursor<PositionT> Cursor;
    private:
        void Setup(Cursor &obj) const override
        {
            SetupOptional(obj, &Cursor::CreateSprite<const SpriteComponent&>, sprite);
            SetupOptional(obj, &Cursor::CreateSpriteFitter<>, spriteFitter);
        }
    public:
        Attribute<SpriteComponent> sprite;
        bool spriteFitter;

        WidgetSetup() = default;
    };

    template<class PositionT>
    using CursorSetup = WidgetSetup<Cursor<PositionT>>;
}