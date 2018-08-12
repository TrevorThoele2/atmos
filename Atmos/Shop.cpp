
#include "Shop.h"

#include "Speech.h"

#include "Entity.h"
#include "AvatarSystem.h"
#include "AvatarComponent.h"
#include "InventoryComponent.h"
#include "DialogueComponent.h"

#include "CurrentField.h"
#include "StringUtility.h"

#include "FontDefines.h"
#include <AGUI\System.h>
#include <AGUI\PushButton.h>

namespace Atmos
{
    namespace Speech
    {
        void Shop::Dialog::OnCountChange()
        {
            price = GetItemPrice(*item->GetWrapped(), ItemStack::CountT(count.Get()));

            itemCountText->SetString(ToString(count.Get()));
            priceText->SetString(ToString(price));
        }

        void Shop::Dialog::Transaction()
        {
            auto avatarComponent = ::Atmos::Ent::AvatarSystem::GetAvatar();
            auto inventoryComponent = ::Atmos::Ent::AvatarSystem::GetInventory();
            if (buying)
            {
                if (avatarComponent->gold >= price)
                {
                    avatarComponent->gold -= price;
                    inventoryComponent->Add(item->GetName(), ItemStack::CountT(count.Get()));
                    return;
                }
            }
            else
            {
                avatarComponent->gold += price;
                item->DecrementCount(ItemStack::CountT(count.Get()));
            }
        }

        Shop::Dialog::Dialog() : active(false), item(nullptr), count(1, 1, 1), price(0)
        {}

        void Shop::Dialog::Init(Agui::ItemDescriptionBox &descBox)
        {
            textbox = Agui::Textbox::Factory(&descBox, "dialog", Agui::RelativePosition(Agui::Dimension(), Agui::Dimension(0, 64), Agui::HorizontalAlignment::MID, Agui::VerticalAlignment::BOT), 0);
            textbox->GetSprite()->color.Edit(255, 100, 255, 100);
            textbox->ScaleTo(256, 64);
            textbox->SetShowWithParent(false);

            itemCountText = &textbox->CreateText("itemCount", 1, Agui::RelativePosition(Agui::HorizontalAlignment::MID, Agui::VerticalAlignment::TOP), Agui::Size(textbox->GetScaledWidth(), 0), Agui::Text("", Agui::Text::CENTER_HORIZONTAL, *Agui::fontSlender, Agui::Color(255, 0, 0, 0))).GetText();
            itemCountText->SetAutoCalcTextHeight();

            priceText = &textbox->CreateText("price", 1, Agui::RelativePosition(Agui::HorizontalAlignment::MID, Agui::VerticalAlignment::BOT), Agui::Size(textbox->GetScaledWidth(), 0), Agui::Text("", Agui::Text::CENTER_HORIZONTAL, *Agui::fontSlender, Agui::Color(255, 0, 0, 0))).GetText();
            priceText->SetAutoCalcTextHeight();

            playerGoldText = &textbox->CreateText("playerGold", 1, Agui::RelativePosition(Agui::HorizontalAlignment::MID, Agui::VerticalAlignment::MID), Agui::Size(textbox->GetScaledWidth(), 0), Agui::Text("", Agui::Text::CENTER_HORIZONTAL, *Agui::fontSlender, Agui::Color(255, 0, 0, 0))).GetText();
            playerGoldText->SetAutoCalcTextHeight();
        }

        void Shop::Dialog::Activate(ItemStack &item)
        {
            active = true;

            this->item = &item;

            auto avatarComponent = ::Atmos::Ent::AvatarSystem::GetAvatar();
            playerGoldText->SetString(ToString(avatarComponent->gold));

            if (buying)
            {
                auto divisor = (*this->item)->GetBuyingPrice();
                if (divisor == 0)
                    divisor = 1;
                count.SetUpperBound(static_cast<unsigned char>(std::floor(avatarComponent->gold / divisor)));
            }
            else
                count.SetUpperBound(this->item->GetCount().Get());

            count.Set(1);
            OnCountChange();
            textbox->Show();
        }

        void Shop::Dialog::Deactivate()
        {
            active = false;
            textbox->Hide();
            Shop::OnDialogDeactivated();
        }

        bool Shop::Dialog::IsActive() const
        {
            return active;
        }

        void Shop::Dialog::OnActionPressed(const Input::Action &args)
        {
            if (!active)
                return;

            switch (args.id)
            {
            case Input::ActionID::NAVIGATE_MENU_DOWN:
                --count;
                OnCountChange();
                break;
            case Input::ActionID::NAVIGATE_MENU_UP:
                ++count;
                OnCountChange();
                break;
            case Input::ActionID::USE:
                Transaction();
                Deactivate();
                break;
            case Input::ActionID::CANCEL:
                Deactivate();
                break;
            }
        }

        bool Shop::active = false;
        bool Shop::buying = false;
        Agui::Root* Shop::root = nullptr;
        Shop::Dialog Shop::dialog;

        InventoryGui Shop::inventoryGui;

        Shop& Shop::Instance()
        {
            static Shop instance;
            return instance;
        }

        void Shop::OnDialogDeactivated()
        {
            inventoryGui.Start();
        }

        void Shop::OnActionPressed(const Input::Action &args)
        {
            if (dialog.IsActive())
            {
                dialog.OnActionPressed(args);
                return;
            }

            switch (args.id)
            {
            case Input::ActionID::USE:
            {
                dialog.Activate(*inventoryGui.GetSelected());
                inventoryGui.Pause();
                break;
            }
            case Input::ActionID::CANCEL:
                Leave();
                break;
            }
        }

        Item::Price Shop::GetItemPrice(const Item &item, const ItemStack::CountT &count)
        {
            return (buying) ? item.GetBuyingPrice() * count.Get() : item.GetSellingPrice() * count.Get();
        }

        void Shop::Init()
        {
            root = Agui::System::CreateRoot("shop");

            // Description box
            auto descBox = Agui::ItemDescriptionBox::Factory(root, "description", Agui::RelativePosition(Agui::Dimension(), Agui::Dimension(0, 128), Agui::HorizontalAlignment::MID, Agui::VerticalAlignment::TOP), 0);
            descBox->GetSprite()->color.Edit(255, 100, 100, 255);
            descBox->ScaleTo(256, 256);

            inventoryGui.Init(*root, Agui::RelativePosition(Agui::HorizontalAlignment::RIGHT, Agui::VerticalAlignment::TOP), "", *descBox, nullptr);
            inventoryGui.GetBackground()->GetSprite()->color.Edit(255, 255, 255, 100);
            inventoryGui.GetBackground()->ScaleTo(282, 568);
            inventoryGui.GetMenu()->GetSprite()->color.Edit(255, 100, 100, 255);
            inventoryGui.GetMenu()->SetPosition(Agui::RelativePosition(Agui::Dimension(), Agui::Dimension(0, 23), Agui::HorizontalAlignment::MID, Agui::VerticalAlignment::TOP));
            inventoryGui.GetMenu()->GetLayout()->ChangeDimensions(230, 503);

            // Leave button
            auto button = Agui::PushButton::Factory(inventoryGui.GetBackground(), "leave", Agui::RelativePosition(Agui::Dimension(), Agui::Dimension(0, -5), Agui::HorizontalAlignment::MID, Agui::VerticalAlignment::BOT), 1);
            button->ModifySprite(Agui::Sprite("Graphics/GUI/buttonSmaller.png", 1, Agui::Color()));
            button->GetText()->SetString("LEAVE");
            button->GetText()->color.Edit(0, 0, 0);
            button->eventClicked.Subscribe(std::bind(&Shop::Leave));

            // Dialog
            dialog.Init(*descBox);
        }

        bool Shop::Enter(bool buying)
        {
            Entity entity = (buying) ? Handler::currentDialogue->GetOwnerEntity() : ::Atmos::Ent::AvatarSystem::GetEntity();
            Ent::InventoryComponent *inventory = GetCurrentEntities()->FindComponent<Ent::InventoryComponent>(entity);

            if (!inventory || inventory->IsEmpty())
            {
                Logger::Log("The shop was attempted to be entered with an entity without an inventory.",
                    Logger::Type::ERROR_LOW,
                    Logger::NameValueVector{ NameValuePair{"Entity", static_cast<size_t>(entity)} });
                return false;
            }

            active = true;
            Shop::buying = buying;
            root->Show();

            inventoryGui.SetList(inventory);
            inventoryGui.Start();
            return true;
        }

        void Shop::Leave()
        {
            active = false;
            root->Hide();
        }

        bool Shop::IsActive()
        {
            return Instance().active;
        }
    }
}