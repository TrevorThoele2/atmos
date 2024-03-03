#pragma once

#include "InventoryComponent.h"
#include "Item.h"

#include "ObjectMenu.h"
#include "ItemDescriptionBox.h"
#include "ItemContextMenu.h"
#include <AGUI\Root.h>
#include <AGUI\Label.h>

namespace Atmos
{
    class InventoryGui : public ObjectMenu<ItemStack, Ent::InventoryComponent, agui::ItemDescriptionBox, agui::ItemContextMenu>
    {
    private:
        agui::Image *background;
        agui::Label *label;

        Ent::InventoryComponent *moveTo;

        InventoryGui(const InventoryGui &arg) = delete;
        void OnGotoContextMenu() override;
    public:
        InventoryGui();
        InventoryGui(agui::Root &root, const agui::RelativePosition &pos, const std::string &labelString, DescriptionBox &description, ContextMenu *contextMenu);
        void Init(agui::Root &root, const agui::RelativePosition &pos, const std::string &labelString, DescriptionBox &description, ContextMenu *contextMenu);
        void SetMoveTo();
        void SetMoveTo(Ent::InventoryComponent &set);
        agui::Image* GetBackground();
        agui::Label* GetLabel();
    };
}