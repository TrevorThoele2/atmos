
#include "Cursor.h"

#include "ObjectManager.h"
#include "InputSystem.h"

namespace Agui
{
    Cursor::Connection::Connection(typename ObjectList::iterator itr, Cursor& owner) : OwnerConnection(itr, owner)
    {}

	void Cursor::Connection::Destroy_impl()
	{
		owner->RemoveEntry(*this);
	}

    Cursor* Cursor::Factory(Object* parent, const std::string& name, const RelativePosition& pos, float z, ::Atmos::ObjectManager& objectManager)
    {
        return CreateBase<Cursor>(parent, name, pos, z, objectManager);
    }

    Cursor* Cursor::Factory(Object* parent, const WidgetPrototype& arg)
    {
        return CreateBase<Cursor>(parent, arg);
    }

    Cursor* Cursor::Factory(Object* parent, const Cursor& arg)
    {
        return CreateBase<Cursor>(parent, arg);
    }

    typename Cursor::Connection Cursor::AddEntry(Object& add)
    {
        ObjectList::iterator added = objectList.end();
        if (objectList.empty())
        {
            added = objectList.AddBack(&add);
            (*added)->AddChild(*this);
        }
        else
            added = objectList.AddBack(&add);

        return Connection(added, *this);
    }

    void Cursor::RemoveEntry(Connection& remove)
    {
        if (remove.owner != this || objectList.empty())
            return;

        RemoveEntry(remove.obj);
    }

    void Cursor::RemoveCurrentEntry()
    {
        RemoveEntry(FindEntry(position));
    }

    void Cursor::ClearEntries()
    {
        RemoveFromParent();
        objectList.Clear();
    }

    void Cursor::SetCurrentEntry(Position set)
    {
        ChangePositionTo(set);
    }

    void Cursor::IncrementCurrentEntry(Position inc)
    {
        SetCurrentEntry(GetCurrentEntry() + inc);
    }

    void Cursor::DecrementCurrentEntry(Position dec)
    {
        SetCurrentEntry(GetCurrentEntry() - dec);
    }

    void Cursor::ResetCurrentEntry()
    {
        ChangePositionTo(0);
    }

    typename Cursor::Position Cursor::GetCurrentEntry() const
    {
        return position;
    }

    Cursor::Cursor(Object* parent, const std::string& name, const RelativePosition& relPos, float z, ::Atmos::ObjectManager& objectManager) :
        SpriteWidgetSync(parent, name, relPos, z), objectManager(&objectManager), position(0)
    {
        Setup();
    }

    Cursor::Cursor(Object* parent, const WidgetPrototype& arg) :
        SpriteWidgetSync(parent, arg), position(0)
    {
        Setup();
    }

    Cursor::Cursor(Object* parent, const Cursor& arg) :
        SpriteWidgetSync(parent, arg), position(arg.position)
    {
        Setup();
    }

	void Cursor::Setup()
	{
        auto input = objectManager->FindSystem<::Atmos::Input::System>();
		input->Get()->eventActionPressed.Subscribe(&Cursor::OnActionPressed, *this);
		SetClippingMethod(ClippingMethod::DONT);
	}

	void Cursor::ParentThisToSelected()
	{
		auto object = *FindEntry(position);
		object->AddChild(*this);
	}

	void Cursor::ChangePositionTo(Position set)
	{
		auto previousPos = position;
		position = set;
		PositionChanged(previousPos);
	}

	void Cursor::ChangePosition(bool add)
	{
        const Position upperBound = static_cast<Position>(objectList.size() - 1);
        const Position lowerBound = 0;

		auto previousPos = position;
		(add) ? ++position : --position;
		if (position > upperBound)
			position = lowerBound;
		else if (position < lowerBound)
			position = upperBound;
		PositionChanged(previousPos);
	}

	void Cursor::PositionChanged(Position previous)
	{
        if (previous == position || objectList.empty())
            return;

		ParentThisToSelected();
		onMoved();
	}

	void Cursor::RemoveEntry(typename ObjectList::iterator itr)
	{
		objectList.Remove(itr);
		ParentThisToSelected();
	}

	Cursor::ObjectList::iterator Cursor::FindEntry(typename ObjectList::ID index)
	{
		return objectList.Find(index);
	}

    void Cursor::OnActionPressed(const Atmos::Input::Action& args)
    {
        if (!IsActive())
            return;

        if (args.id == Atmos::Input::ActionID::NAVIGATE_MENU_UP)
            OnNavigateMenuUp();
        else if (args.id == Atmos::Input::ActionID::NAVIGATE_MENU_DOWN)
            OnNavigateMenuDown();
    }

    void Cursor::OnNavigateMenuUp()
    {
        ChangePosition(false);
    }

    void Cursor::OnNavigateMenuDown()
    {
        ChangePosition(true);
    }
}