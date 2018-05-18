
#include "ItemContextMenu.h"

#include "Entity.h"
#include "InventoryComponent.h"
#include "Consumable.h"

#include "FontDefines.h"

namespace agui
{
    void ItemContextMenu::RegisterInputs()
    {
        RegisterInput(Text("CONSUME", agui::Text::Format(), *agui::fontSlender, agui::colorBlack), RelativePosition(Dimension(0, 26), Dimension(0, 5)), std::bind(&ItemContextMenu::OnConsume, this));
        RegisterInput(Text("EQUIP", agui::Text::Format(), *agui::fontSlender, agui::colorBlack), RelativePosition(Dimension(0, 26), Dimension(0, 25)), std::bind(&ItemContextMenu::OnEquip, this));
        RegisterInput(Text("MOVE", agui::Text::Format(), *agui::fontSlender, agui::colorBlack), RelativePosition(Dimension(0, 26), Dimension(0, 45)), std::bind(&ItemContextMenu::OnMove, this));
    }

    void ItemContextMenu::OnConsume()
    {
        moveFrom->AttemptConsume(*itemSelected);
    }

    void ItemContextMenu::OnEquip()
    {
        moveFrom->AttemptEquip(Atmos::EquipSlot::MAINHAND, *itemSelected);
    }

    void ItemContextMenu::OnMove()
    {
        moveFrom->Move(*itemSelected, *moveTo);
    }

    ItemContextMenu::ItemContextMenu(Object *parent, const std::string &name, const RelativePosition &pos, float z) : ContextMenu(parent, name, pos, z), itemSelected(nullptr), moveFrom(nullptr), moveTo(nullptr)
    {}

    ItemContextMenu::ItemContextMenu(Object *parent, const WidgetPrototype &arg) : ContextMenu(parent, arg), itemSelected(nullptr), moveFrom(nullptr), moveTo(nullptr)
    {}

    ItemContextMenu::ItemContextMenu(Object *parent, const ItemContextMenu &arg) : ContextMenu(parent, arg), itemSelected(arg.itemSelected), moveFrom(arg.moveFrom), moveTo(arg.moveTo)
    {}

    ItemContextMenu* ItemContextMenu::Factory(Object *parent, const std::string &name, const RelativePosition &pos, float z)
    {
        return CreateBase<ItemContextMenu>(parent, name, pos, z);
    }

    ItemContextMenu* ItemContextMenu::Factory(Object *parent, const WidgetPrototype &arg)
    {
        return CreateBase<ItemContextMenu>(parent, arg);
    }

    ItemContextMenu* ItemContextMenu::Factory(Object *parent, const ItemContextMenu &arg)
    {
        return CreateBase<ItemContextMenu>(parent, arg);
    }

    void ItemContextMenu::SetSelected()
    {
        itemSelected = nullptr;
    }

    void ItemContextMenu::SetSelected(Atmos::ItemStack &set)
    {
        itemSelected = &set;
    }

    void ItemContextMenu::SetMoveFrom(Atmos::Ent::InventoryComponent &set)
    {
        moveFrom = &set;
    }

    void ItemContextMenu::SetMoveTo(Atmos::Ent::InventoryComponent &set)
    {
        moveTo = &set;
    }

    void ItemContextMenu::FlipMoving()
    {
        auto temp = moveFrom;
        moveFrom = moveTo;
        moveTo = temp;
    }
}