
#pragma once

#include "ContextMenu.h"
#include "ItemStack.h"

namespace Atmos
{
    namespace Ent
    {
        class InventoryComponent;
    }
}

namespace Agui
{
    class ItemContextMenu : public ContextMenu
    {
    private:
        Atmos::ItemStack *itemSelected;
        Atmos::Ent::InventoryComponent *moveFrom, *moveTo;

        void RegisterInputs() override;
        void OnConsume();
        void OnEquip();
        void OnMove();
    protected:
        ItemContextMenu(Object *parent, const std::string &name, const RelativePosition &pos, float z);
        ItemContextMenu(Object *parent, const WidgetPrototype &arg);
        ItemContextMenu(Object *parent, const ItemContextMenu &arg);
    public:
        static ItemContextMenu* Factory(Object *parent, const std::string &name, const RelativePosition &pos, float z);
        static ItemContextMenu* Factory(Object *parent, const WidgetPrototype &arg);
        static ItemContextMenu* Factory(Object *parent, const ItemContextMenu &arg);
        void SetSelected();
        void SetSelected(Atmos::ItemStack &set);
        void SetMoveFrom(Atmos::Ent::InventoryComponent &set);
        void SetMoveTo(Atmos::Ent::InventoryComponent &set);
        void FlipMoving();
        WIDGET_COMMON_ACCESS(ItemContextMenu);
    };

    template<>
    class WidgetSetup<ItemContextMenu> : public WidgetSetupBase<ItemContextMenu>
    {
    private:
        void Setup(WidgetType &obj) const override
        {
            SetupOptional(obj, &WidgetType::SetMenu, menu);
            SetupOptional(obj, &WidgetType::SetCursor, cursor);
        }
    public:
        WidgetAttribute<Menu> menu;
		WidgetAttribute<Cursor> cursor;

        WidgetSetup() = default;
    };

    typedef WidgetSetup<ItemContextMenu> ItemContextMenuSetup;
}