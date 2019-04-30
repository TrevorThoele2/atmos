
#include "Shop.h"

#include "SpeechController.h"

#include "EntityAvatarSystem.h"
#include "AvatarComponent.h"
#include "InventoryComponent.h"
#include "ActionComponent.h"

#include "InputSystem.h"
#include "LoggingSystem.h"
#include "StringUtility.h"

#include "FontDefines.h"
#include <AGUI/System.h>
#include <AGUI/PushButton.h>

namespace Atmos
{
    namespace Speech
    {
        Shop::Dialog::Dialog(Shop& owner) : active(false), owner(&owner), focusedItemStack(nullptr), count(1, 1, 1), price(0)
        {}

        void Shop::Dialog::Initialize(Agui::ItemDescriptionBox& descBox)
        {
            textbox = Agui::Textbox::Factory(
                &descBox,
                "dialog",
                Agui::RelativePosition(Agui::Dimension(), Agui::Dimension(0, 64), Agui::HorizontalAlignment::MID, Agui::VerticalAlignment::BOT),
                0);
            textbox->GetSprite()->color.Edit(255, 100, 255, 100);
            textbox->ScaleTo(256, 64);
            textbox->SetShowWithParent(false);

            itemCountText = &textbox->CreateText(
                "itemCount",
                1,
                Agui::RelativePosition(Agui::HorizontalAlignment::MID, Agui::VerticalAlignment::TOP),
                Agui::Size(textbox->GetScaledWidth(), 0),
                Agui::Text("", Agui::Text::CENTER_HORIZONTAL, *Agui::fontSlender, Agui::Color(255, 0, 0, 0))).GetText();
            itemCountText->SetAutoCalcTextHeight();

            priceText = &textbox->CreateText(
                "price",
                1,
                Agui::RelativePosition(Agui::HorizontalAlignment::MID, Agui::VerticalAlignment::BOT),
                Agui::Size(textbox->GetScaledWidth(), 0),
                Agui::Text("", Agui::Text::CENTER_HORIZONTAL, *Agui::fontSlender, Agui::Color(255, 0, 0, 0))).GetText();
            priceText->SetAutoCalcTextHeight();

            playerGoldText = &textbox->CreateText(
                "playerGold",
                1,
                Agui::RelativePosition(Agui::HorizontalAlignment::MID, Agui::VerticalAlignment::MID),
                Agui::Size(textbox->GetScaledWidth(), 0),
                Agui::Text("", Agui::Text::CENTER_HORIZONTAL, *Agui::fontSlender, Agui::Color(255, 0, 0, 0))).GetText();
            playerGoldText->SetAutoCalcTextHeight();
        }

        void Shop::Dialog::Activate(ItemStack& withItem)
        {
            active = true;

            focusedItemStack = &withItem;

            auto avatarComponent = owner->AvatarComponent();
            playerGoldText->SetString(ToString(avatarComponent->currency));

            if (owner->isBuying)
            {
                auto divisor = focusedItemStack->itemSource->buyPrice;
                if (divisor == 0)
                    divisor = 1;
                count.SetUpperBound(static_cast<unsigned char>(std::floor(avatarComponent->currency / divisor)));
            }
            else
                count.SetUpperBound(focusedItemStack->count.Value());

            count.Set(1);
            OnCountChange();
            textbox->Show();
        }

        void Shop::Dialog::Deactivate()
        {
            active = false;
            textbox->Hide();
            owner->OnDialogDeactivated();
        }

        bool Shop::Dialog::IsActive() const
        {
            return active;
        }

        void Shop::Dialog::OnActionPressed(const Input::Action& args)
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

        void Shop::Dialog::OnCountChange()
        {
            price = owner->ItemPrice(focusedItemStack->itemSource.Get(), ItemStack::Count(count.Value()));

            itemCountText->SetString(ToString(count.Value()));
            priceText->SetString(ToString(price));
        }

        void Shop::Dialog::Transaction()
        {
            auto avatarComponent = owner->AvatarComponent();
            auto inventoryComponent = owner->AvatarInventoryComponent();
            if (owner->isBuying)
            {
                if (avatarComponent->currency >= price)
                {
                    avatarComponent->currency -= price;
                    inventoryComponent->Add(focusedItemStack->itemSource, ItemStack::Count(count.Value()));
                    return;
                }
            }
            else
            {
                avatarComponent->currency += price;
                focusedItemStack->count -= ItemStack::Count(count.Value());
            }
        }

        Shop::Shop(ObjectManager& manager) : ObjectSystem(manager), active(false), controller(nullptr), isBuying(false), root(nullptr), dialog(*this)
        {
            auto input = manager.FindSystem<Input::System>();
            input->Get()->eventActionPressed.Subscribe(&Shop::OnActionPressed, *this);
        }

        bool Shop::Enter(bool buying)
        {
            EntityReference entity = (buying) ? controller->currentDialogue->owner : Avatar();
            InventoryComponentReference inventory = InventoryComponent(entity);

            if (!inventory || inventory->IsEmpty())
            {
                FindLoggingSystem()->Log("The shop was attempted to be entered with an entity without an inventory.",
                    LogType::ERROR_LOW,
                    LogNameValueVector{ NameValuePair{"Entity", entity->ID()} });
                return false;
            }

            active = true;
            isBuying = buying;
            root->Show();

            return true;
        }

        void Shop::Leave()
        {
            active = false;
            root->Hide();
        }

        bool Shop::IsActive() const
        {
            return active;
        }

        void Shop::InitializeImpl()
        {
            controller = Manager()->FindSystem<Controller>();
            InitializeGui();
        }

        nItem::Price Shop::ItemPrice(TypedObjectReference<nItem> item, const ItemStack::Count& count)
        {
            return (isBuying) ? item->buyPrice * count.Value() : item->sellPrice * count.Value();
        }

        void Shop::InitializeGui()
        {
            root = Agui::System::CreateRoot("shop");

            // Description box
            auto descBox = Agui::ItemDescriptionBox::Factory(
                root,
                "description",
                Agui::RelativePosition(Agui::Dimension(), Agui::Dimension(0, 128), Agui::HorizontalAlignment::MID, Agui::VerticalAlignment::TOP),
                0);
            descBox->GetSprite()->color.Edit(255, 100, 100, 255);
            descBox->ScaleTo(256, 256);

            // Dialog
            dialog.Initialize(*descBox);
        }

        Shop::EntityReference Shop::Avatar()
        {
            return Manager()->FindSystem<Entity::AvatarSystem>()->Avatar();
        }

        Shop::AvatarComponentReference Shop::AvatarComponent()
        {
            auto avatar = Avatar();
            auto avatarComponent = avatar->RetrieveComponent<Entity::AvatarComponent>();
            return avatarComponent;
        }

        Shop::InventoryComponentReference Shop::AvatarInventoryComponent()
        {
            auto avatar = Avatar();
            return InventoryComponent(avatar);
        }

        Shop::InventoryComponentReference Shop::InventoryComponent(EntityReference entity)
        {
            return entity->RetrieveComponent<Entity::InventoryComponent>();
        }

        void Shop::OnDialogDeactivated()
        {}

        void Shop::OnActionPressed(const Input::Action& args)
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
                break;
            }
            case Input::ActionID::CANCEL:
                Leave();
                break;
            }
        }

        LoggingSystem* Shop::FindLoggingSystem()
        {
            return Manager()->FindSystem<LoggingSystem>();
        }
    }
}