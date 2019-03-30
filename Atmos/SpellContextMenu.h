
#pragma once

#include "ContextMenu.h"

#include "Spell.h"
#include "CombatComponent.h"
#include "ObjectReference.h"

namespace Agui
{
    class SpellContextMenu : public ContextMenu
    {
    private:
        ::Atmos::TypedObjectReference<::Atmos::nSpell> selected;
        ::Atmos::TypedObjectReference<::Atmos::Ent::nCombatComponent> source;

        void RegisterInputs() override;
        void OnCast();
    protected:
        SpellContextMenu(Object *parent, const std::string &name, const RelativePosition &pos, float z);
        SpellContextMenu(Object *parent, const WidgetPrototype &arg);
        SpellContextMenu(Object *parent, const SpellContextMenu &arg);
    public:
        static SpellContextMenu* Factory(Object *parent, const std::string &name, const RelativePosition &pos, float z);
        static SpellContextMenu* Factory(Object *parent, const WidgetPrototype &arg);
        static SpellContextMenu* Factory(Object *parent, const SpellContextMenu &arg);
        void SetSelected();
        void SetSelected(::Atmos::TypedObjectReference<::Atmos::nSpell> set);
        void SetSource(::Atmos::TypedObjectReference<::Atmos::Ent::nCombatComponent> set);
        WIDGET_COMMON_ACCESS(SpellContextMenu);
    };

    template<>
    class WidgetSetup<SpellContextMenu> : public WidgetSetupBase<SpellContextMenu>
    {
    private:
        void Setup(SpellContextMenu &obj) const override
        {
            SetupOptional(obj, &SpellContextMenu::SetMenu, menu);
            SetupOptional(obj, &SpellContextMenu::SetCursor, cursor);
        }
    public:
        WidgetAttribute<Menu> menu;
        WidgetAttribute<Cursor> cursor;

        WidgetSetup() = default;
    };

    typedef WidgetSetup<SpellContextMenu> SpellContextMenuSetup;
}