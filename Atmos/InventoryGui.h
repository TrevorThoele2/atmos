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
    class InventoryGui : public ObjectMenu<ItemStack, Ent::InventoryComponent, Agui::ItemDescriptionBox, Agui::ItemContextMenu>
    {
    private:
        Agui::Image *background;
        Agui::Label *label;

        Ent::InventoryComponent *moveTo;

        InventoryGui(const InventoryGui &arg) = delete;
        void OnGotoContextMenu() override;
    public:
        InventoryGui();
        InventoryGui(Agui::Root &root, const Agui::RelativePosition &pos, const std::string &labelString, DescriptionBox &description, ContextMenu *contextMenu);
        void Init(Agui::Root &root, const Agui::RelativePosition &pos, const std::string &labelString, DescriptionBox &description, ContextMenu *contextMenu);
        void SetMoveTo();
        void SetMoveTo(Ent::InventoryComponent &set);
        Agui::Image* GetBackground();
        Agui::Label* GetLabel();
    };
}