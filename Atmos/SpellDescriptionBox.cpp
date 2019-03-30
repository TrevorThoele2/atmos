
#include "SpellDescriptionBox.h"

#include "Spell.h"

namespace Agui
{
    void SpellDescriptionBox::GetNameString(::std::string &string) const
    {
        if(!focusedSpell.IsOccupied())
            string = "";
        else
            string = focusedSpell->name;
    }

    void SpellDescriptionBox::GetDescriptionString(::std::string &string) const
    {
        if(!focusedSpell.IsOccupied())
            string = "";
        else
            string = focusedSpell->description;
    }

    SpellDescriptionBox::SpellDescriptionBox(Object *parent, const std::string &name, const RelativePosition &pos, float z) : DescriptionBox(parent, name, pos, z)
    {}

    SpellDescriptionBox::SpellDescriptionBox(Object *parent, const WidgetPrototype &arg) : DescriptionBox(parent, arg)
    {}

    SpellDescriptionBox::SpellDescriptionBox(Object *parent, const SpellDescriptionBox &arg) : DescriptionBox(parent, arg)
    {}

    SpellDescriptionBox* SpellDescriptionBox::Factory(Object *parent, const std::string &name, const RelativePosition &pos, float z)
    {
        return CreateBase<SpellDescriptionBox>(parent, name, pos, z);
    }

    SpellDescriptionBox* SpellDescriptionBox::Factory(Object *parent, const WidgetPrototype &arg)
    {
        return CreateBase<SpellDescriptionBox>(parent, arg);
    }

    SpellDescriptionBox* SpellDescriptionBox::Factory(Object *parent, const SpellDescriptionBox &arg)
    {
        return CreateBase<SpellDescriptionBox>(parent, arg);
    }

    void SpellDescriptionBox::Set()
    {
        focusedSpell.Reset();
        Refresh();
    }

    void SpellDescriptionBox::Set(SpellReference set)
    {
        focusedSpell = set;
        Refresh();
    }

    WidgetSetup<SpellDescriptionBox>::WidgetSetup() : spriteFitter(false)
    {}
}