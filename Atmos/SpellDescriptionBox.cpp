
#include "SpellDescriptionBox.h"

#include "Spell.h"

namespace Agui
{
    void SpellDescriptionBox::GetNameString(::std::string &string) const
    {
        if(!selected)
            string = "";
        else
            string = selected->GetName();
    }

    void SpellDescriptionBox::GetDescriptionString(::std::string &string) const
    {
        if(!selected)
            string = "";
        else
            string = selected->GetDescription();
    }

    SpellDescriptionBox::SpellDescriptionBox(Object *parent, const std::string &name, const RelativePosition &pos, float z) : DescriptionBox(parent, name, pos, z), selected(nullptr)
    {}

    SpellDescriptionBox::SpellDescriptionBox(Object *parent, const WidgetPrototype &arg) : DescriptionBox(parent, arg), selected(nullptr)
    {}

    SpellDescriptionBox::SpellDescriptionBox(Object *parent, const SpellDescriptionBox &arg) : DescriptionBox(parent, arg), selected(nullptr)
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
        selected = nullptr;
        Refresh();
    }

    void SpellDescriptionBox::Set(const Atmos::Spell &set)
    {
        selected = &set;
        Refresh();
    }

    WidgetSetup<SpellDescriptionBox>::WidgetSetup() : spriteFitter(false)
    {}
}