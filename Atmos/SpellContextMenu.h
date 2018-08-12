
#pragma once

#include "ContextMenu.h"

namespace Atmos
{
    class Spell;
    namespace Ent
    {
        class CombatComponent;
    }
}

namespace Agui
{
    class SpellContextMenu : public ContextMenu
    {
    private:
        const Atmos::Spell *selected;
        const Atmos::Ent::CombatComponent *source;
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
        void SetSelected(const Atmos::Spell &set);
        void SetSource(const Atmos::Ent::CombatComponent &set);
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