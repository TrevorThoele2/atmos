
#pragma once

#include "ItemDescriptionBox.h"
#include "ItemStack.h"
#include "InventoryGui.h"
#include "Input.h"

#include <AGUI\Menu.h>
#include <AGUI\Textbox.h>

namespace Atmos
{
    namespace Speech
    {
        class Handler;
        class Shop
        {
        public:
            class Dialog
            {
            private:
                bool active;
                agui::Textbox *textbox;
                agui::TextComponent *itemCountText, *priceText, *playerGoldText;

                ItemStack *item;
                DynamicBoundedNumber<unsigned char> count;
                Item::Price price;

                void OnCountChange();
                void Transaction();
            public:
                Dialog();
                void Init(agui::ItemDescriptionBox &descBox);
                void Activate(ItemStack &item);
                void Deactivate();
                bool IsActive() const;

                void OnActionPressed(const Input::Action &args);
            };

        private:
            static bool active;
            static bool buying;
            static agui::Root *root;
            static Dialog dialog;

            static InventoryGui inventoryGui;

            Shop() = default;
            Shop(const Shop &arg) = delete;
            Shop& operator=(const Shop &arg) = delete;

            static Shop& Instance();

            static void OnDialogDeactivated();
            static void OnActionPressed(const Input::Action &args);

            static Item::Price GetItemPrice(const Item &item, const ItemStack::CountT &count);
        public:
            static void Init();
            static bool Enter(bool buying);
            static void Leave();
            static bool IsActive();
            friend Handler;
        };
    }
}