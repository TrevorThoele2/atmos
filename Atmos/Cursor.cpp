
#include "Cursor.h"

namespace Agui
{
	void Cursor::Connection::Destroy_impl()
	{
		owner->RemoveEntry(*this);
	}

	Cursor::Connection::Connection(typename Objects::iterator itr, Cursor &owner) : OwnerConnection(itr, owner)
	{}

	void Cursor::Setup()
	{
		::Atmos::Environment::GetInput()->eventActionPressed.Subscribe(&Cursor::OnActionPressed, *this);
		SetClippingMethod(ClippingMethod::DONT);
	}

	void Cursor::ParentThisToSelected()
	{
		auto object = *FindEntry(pos);
		object->AddChild(*this);
	}

	void Cursor::ChangePosTo(Position set)
	{
		auto previousPos = pos;
		pos = set;
		PosChanged(previousPos);
	}

	void Cursor::ChangePos(bool add)
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

	void Cursor::PosChanged(Position prev)
	{
		if (prev != pos && !objects.empty())
		{
			ParentThisToSelected();
			eventMoved();
		}
	}

	void Cursor::OnActionPressed(const Atmos::Input::Action &args)
	{
		if (!IsActive())
			return;

		if (args.id == Atmos::Input::ActionID::NAVIGATE_MENU_UP)
			OnNavMenuUp();
		else if (args.id == Atmos::Input::ActionID::NAVIGATE_MENU_DOWN)
			OnNavMenuDown();
	}

	void Cursor::OnNavMenuUp()
	{
		ChangePos(false);
	}

	void Cursor::OnNavMenuDown()
	{
		ChangePos(true);
	}

	void Cursor::RemoveEntry(typename Objects::iterator itr)
	{
		objects.Remove(itr);
		ParentThisToSelected();
	}

	Cursor::Objects::iterator Cursor::FindEntry(typename Objects::ID index)
	{
		return objects.Find(index - 1);
	}

	Cursor::Cursor(Object *parent, const std::string &name, const RelativePosition &relPos, float z) : SpriteWidgetSync(parent, name, relPos, z), pos(1)
	{
		Setup();
	}

	Cursor::Cursor(Object *parent, const WidgetPrototype &arg) : SpriteWidgetSync(parent, arg), pos(1)
	{
		Setup();
	}

	Cursor::Cursor(Object *parent, const Cursor &arg) : SpriteWidgetSync(parent, arg), pos(arg.pos)
	{
		Setup();
	}

	Cursor* Cursor::Factory(Object *parent, const std::string &name, const RelativePosition &pos, float z)
	{
		return CreateBase<Cursor>(parent, name, pos, z);
	}

	Cursor* Cursor::Factory(Object *parent, const WidgetPrototype &arg)
	{
		return CreateBase<Cursor>(parent, arg);
	}

	Cursor* Cursor::Factory(Object *parent, const Cursor &arg)
	{
		return CreateBase<Cursor>(parent, arg);
	}

	typename Cursor::Connection Cursor::AddEntry(Object &add)
	{
		Objects::iterator added = objects.end();
		if (objects.empty())
		{
			added = objects.AddBack(&add);
			(*added)->AddChild(*this);
		}
		else
			added = objects.AddBack(&add);

		return Connection(added, *this);
	}

	void Cursor::RemoveEntry(Connection &remove)
	{
		if (remove.owner != this || objects.empty())
			return;

		RemoveEntry(remove.obj);
	}

	void Cursor::RemoveCurrentEntry()
	{
		RemoveEntry(FindEntry(pos));
	}

	void Cursor::ClearEntries()
	{
		RemoveFromParent();
		objects.Clear();
	}

	void Cursor::SetCurrentEntry(Position set)
	{
		ChangePosTo(set);
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
		ChangePosTo(1);
	}

	typename Cursor::Position Cursor::GetCurrentEntry() const
	{
		return pos;
	}
}