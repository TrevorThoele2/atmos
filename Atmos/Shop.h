
#pragma once

#include "ObjectSystem.h"

#include "ItemDescriptionBox.h"
#include "ItemStack.h"
#include "InputAction.h"

#include "AvatarComponent.h"
#include "InventoryComponent.h"

#include <AGUI/Menu.h>
#include <AGUI/Textbox.h>
#include <AGUI/Root.h>

namespace Atmos
{
    class LoggingSystem;

    namespace Speech
    {
        class Controller;

        class Shop : public ObjectSystem
        {
        public:
            class Dialog
            {
            public:
                Dialog(Shop& owner);
                void Initialize(Agui::ItemDescriptionBox& descBox);
                void Activate(ItemStack& withItem);
                void Deactivate();
                bool IsActive() const;

                void OnActionPressed(const Input::Action& args);
            private:
                bool active;
            private:
                Shop* owner;
            private:
                Agui::Textbox* textbox;
                Agui::TextComponent* itemCountText;
                Agui::TextComponent* priceText;
                Agui::TextComponent* playerGoldText;
            private:
                ItemStack* focusedItemStack;
                DynamicBoundedNumber<unsigned char> count;
                nItem::Price price;

                void OnCountChange();
                void Transaction();
            };
        public:
            Shop(ObjectManager& manager);

            bool Enter(bool buying);
            void Leave();
            bool IsActive() const;
        private:
            bool active;
        private:
            Controller* controller;
        private:
            void InitializeImpl() override;
        private:
            bool isBuying;
            nItem::Price ItemPrice(TypedObjectReference<nItem> item, const ItemStack::Count& count);
        private:
            Agui::Root* root;
            Dialog dialog;
            void InitializeGui();
        private:
            typedef TypedObjectReference<Entity::Entity> EntityReference;
            typedef TypedObjectReference<Entity::AvatarComponent> AvatarComponentReference;
            typedef TypedObjectReference<Entity::InventoryComponent> InventoryComponentReference;

            EntityReference Avatar();
            AvatarComponentReference AvatarComponent();
            InventoryComponentReference AvatarInventoryComponent();

            InventoryComponentReference InventoryComponent(EntityReference entity);
        private:
            void OnDialogDeactivated();
            void OnActionPressed(const Input::Action& args);
        private:
            LoggingSystem* FindLoggingSystem();
        };
    }
}