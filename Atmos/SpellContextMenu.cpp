
#include "SpellContextMenu.h"

#include "Spell.h"
#include "FontDefines.h"

namespace Agui
{
    void SpellContextMenu::RegisterInputs()
    {
        RegisterInput(Text("CAST", Agui::Text::Format(), *Agui::fontSlender, Color(255, 0, 0, 0)), RelativePosition(Dimension(0, 26), Dimension(0, 5)), std::bind(&SpellContextMenu::OnCast, this));
    }

    void SpellContextMenu::OnCast()
    {

    }

    SpellContextMenu::SpellContextMenu(Object *parent, const std::string &name, const RelativePosition &pos, float z) : ContextMenu(parent, name, pos, z), selected(nullptr), source(nullptr)
    {}

    SpellContextMenu::SpellContextMenu(Object *parent, const WidgetPrototype &arg) : ContextMenu(parent, arg), selected(nullptr), source(nullptr)
    {}

    SpellContextMenu::SpellContextMenu(Object *parent, const SpellContextMenu &arg) : ContextMenu(parent, arg), selected(nullptr), source(nullptr)
    {}

    SpellContextMenu* SpellContextMenu::Factory(Object *parent, const std::string &name, const RelativePosition &pos, float z)
    {
        return CreateBase<SpellContextMenu>(parent, name, pos, z);
    }

    SpellContextMenu* SpellContextMenu::Factory(Object *parent, const WidgetPrototype &arg)
    {
        return CreateBase<SpellContextMenu>(parent, arg);
    }

    SpellContextMenu* SpellContextMenu::Factory(Object *parent, const SpellContextMenu &arg)
    {
        return CreateBase<SpellContextMenu>(parent, arg);
    }

    void SpellContextMenu::SetSelected()
    {
        selected = nullptr;
    }

    void SpellContextMenu::SetSelected(::Atmos::TypedObjectReference<::Atmos::nSpell> set)
    {
        selected = set;
    }

    void SpellContextMenu::SetSource(::Atmos::TypedObjectReference<::Atmos::Ent::nCombatComponent> set)
    {
        source = set;
    }
}