
#include <functional>

#include "StatusScreen.h"
#include "AvatarSystem.h"
#include "Spell.h"

#include "GeneralComponent.h"
#include "SenseComponent.h"
#include "CombatComponent.h"

#include "CurrentField.h"
#include "StringUtility.h"
#include "WrapMath.h"

#include "Assert.h"

#include "FontDefines.h"
#include <AGUI\System.h>
#include <AGUI\Image.h>
#include <AGUI\Label.h>
#include <AGUI\SequenceLayout.h>
#include <AGUI\UniformGridLayout.h>
#include <AGUI\PushButton.h>

namespace Atmos
{
    Agui::Root* StatusScreen::Page::GetRoot()
    {
        return root;
    }

    bool StatusScreen::Page::IsSelected() const
    {
        return StatusScreen::currentPage->second.get() == this;
    }

    StatusScreen::Page::Page(const std::string &name, Agui::Root &mainRoot)
    {
        root = Agui::Root::Factory(&mainRoot, name);
    }

    void StatusScreen::Page::Select(Entity selected)
    {
        root->Show();
        OnSelected();
        Update(selected);
    }

    void StatusScreen::Page::Deselect()
    {
        root->Hide();
        OnDeselected();
    }

    StatusScreen::Stats::Entry::Entry(Agui::TextComponent &component, const Name &niceName) : component(&component), niceName(niceName), type(Type::NAME)
    {}

    StatusScreen::Stats::Entry::Entry(Agui::TextComponent &component, const Name &attributeName, bool isStat) : component(&component), attributeName(attributeName)
    {
        if (isStat)
        {
            niceName = *DataStandard<StatAttributeTable>::GetNiceName(attributeName);
            type = Type::STAT;
        }
        else
        {
            niceName = *DataStandard<ResourceAttributeTable>::GetNiceName(attributeName);
            type = Type::RESOURCE;
        }
    }

    void StatusScreen::Stats::Entry::Update(const Ent::CombatComponent *selected)
    {
        if (!selected)
        {
            component->SetString(niceName);
            return;
        }

        if (type == Type::NAME)
        {
            component->SetString(niceName + GetCurrentEntities()->FindComponent<Ent::GeneralComponent>(selected->GetOwnerEntity())->name);
            return;
        }

        component->SetString(niceName + ToString((type == Type::STAT) ? *selected->stats.GetValue(attributeName) : *selected->resources.GetValue(attributeName)));
    }

    void StatusScreen::Stats::CreateEntry(Agui::Menu &menu)
    {
        auto connection = menu.CreateText("name", 1, Agui::Text("", Agui::Text::CENTER_HORIZONTAL, *Agui::fontButton, Agui::Color(255, 0, 0, 0)));
        connection.GetText().SetAutoCalcTextSize();
        entries.push_back(Entry(connection.GetText(), "Name"));
    }

    void StatusScreen::Stats::CreateEntry(Agui::Menu &menu, const Name &attributeName, bool isStat)
    {
        auto connection = menu.CreateText(attributeName, 1, Agui::Text("", Agui::Text::CENTER_HORIZONTAL, *Agui::fontButton, Agui::Color(255, 0, 0, 0)));
        connection.GetText().SetAutoCalcTextSize();
        entries.push_back(Entry(connection.GetText(), attributeName, isStat));
    }

    StatusScreen::Stats::Stats(Agui::Root &mainRoot) : Page("stats", mainRoot)
    {
        // Character status background
        auto image = Agui::Image::Factory(GetRoot(), "background", Agui::RelativePosition(Agui::HorizontalAlignment::RIGHT, Agui::VerticalAlignment::TOP), 0);
        image->ScaleTo(256, 768);
        image->GetSprite()->color.Edit(255, 0, 0, 0);

        // Character status label
        auto label = Agui::Label::Factory(image, "label", Agui::RelativePosition(Agui::Dimension(), Agui::Dimension(0, 2), Agui::HorizontalAlignment::MID, Agui::VerticalAlignment::TOP), 0);
        label->GetText()->SetString("STATUS:");
        label->GetText()->color.Edit(255, 255, 255, 255);
        label->GetText()->SetAutoCalcTextSize();

        // Character status
        auto menu = Agui::Menu::Factory(image, "characterStatus", Agui::RelativePosition(Agui::Dimension(), Agui::Dimension(0, -5), Agui::HorizontalAlignment::MID, Agui::VerticalAlignment::BOT), 0);
        menu->GetLayout()->SetSelfModifySize(false);
        menu->GetLayout()->ChangeDimensions(246, 738);
        menu->GetSprite()->color.Edit(255, 255, 255, 255);

        // Character stats
        CreateEntry(*menu);
        for (auto &loop : DataStandard<ResourceAttributeTable>::GetNames())
            CreateEntry(*menu, loop, false);
        for (auto &loop : DataStandard<StatAttributeTable>::GetNames())
            CreateEntry(*menu, loop, true);
    }

    void StatusScreen::Stats::Update(Entity selected)
    {
        const Ent::CombatComponent *use = nullptr;
        if (selected)
        {
            auto found = GetCurrentEntities()->FindComponent<Ent::CombatComponent>(selected);
            if (found)
                use = found;
        }

        for (auto loop = entries.begin(); loop != entries.end(); loop++)
            loop->Update(use);
    }

    StatusScreen::Inventory::SectionBase::SectionBase(Agui::Root &root, const Agui::RelativePosition &position, Agui::ItemDescriptionBox &description, Agui::ItemContextMenu &contextMenu, const std::string &labelString, Inventory &handler) : handler(handler), gui(root, position, labelString, description, &contextMenu), other(nullptr)
    {}

    void StatusScreen::Inventory::SectionBase::Focus()
    {
        gui.Start();
        OnFocus();
    }

    void StatusScreen::Inventory::SectionBase::Unfocus()
    {
        gui.Pause();
        OnUnfocus();
    }

    void StatusScreen::Inventory::CharacterSection::OnFocus()
    {
        gui.SetMoveTo(*Ent::AvatarSystem::GetInventory());
    }

    StatusScreen::Inventory::CharacterSection::CharacterSection(Agui::Root &root, const Agui::RelativePosition &position, Agui::ItemDescriptionBox &description, Agui::ItemContextMenu &contextMenu, Inventory &handler) : SectionBase(root, position, description, contextMenu, "INVENTORY:", handler)
    {}

    void StatusScreen::Inventory::CharacterSection::Update(Entity selected)
    {
        if (selected)
            gui.SetList(GetCurrentEntities()->FindComponent<Ent::InventoryComponent>(selected));
        else
            gui.SetList(nullptr);
    }

    StatusScreen::Inventory::ReserveSection::ReserveSection(Agui::Root &root, const Agui::RelativePosition &position, Agui::ItemDescriptionBox &description, Agui::ItemContextMenu &contextMenu, Inventory &handler) : SectionBase(root, position, description, contextMenu, "RESERVE INVENTORY:", handler)
    {}

    void StatusScreen::Inventory::ReserveSection::Update(Entity selected)
    {
        if (selected)
            gui.SetMoveTo(*GetCurrentEntities()->FindComponent<Ent::InventoryComponent>(selected));
        else
            gui.SetMoveTo();
    }

    void StatusScreen::Inventory::ReserveSection::Setup()
    {
        gui.SetList(Ent::AvatarSystem::GetInventory());
    }

    void StatusScreen::Inventory::OnSelected()
    {
        SelectSection(sections.begin());
        for (auto &loop : sections)
            loop->Setup();

        for (auto loop = std::next(sections.begin()); loop != sections.end(); loop++)
            (*loop)->Unfocus();
    }

    void StatusScreen::Inventory::OnDeselected()
    {
        for (auto &loop : sections)
            loop->Unfocus();
    }

    void StatusScreen::Inventory::OnSectionUpdated()
    {
        ATMOS_ASSERT(selectedSection != sections.end());

        boundSubscribers.Unsubscribe();
        (*selectedSection)->other->gui.Update();
        boundSubscribers.Subscribe();
    }

    void StatusScreen::Inventory::IncSelectedSection()
    {
        auto next = std::next(selectedSection);
        if (next == sections.end())
            next = sections.begin();

        SelectSection(next);
    }

    void StatusScreen::Inventory::SelectSection(Sections::iterator select)
    {
        (*selectedSection)->Unfocus();
        selectedSection = select;
        (*selectedSection)->Focus();
    }

    StatusScreen::Inventory::Inventory(Agui::Root &mainRoot) : Page("inventory", mainRoot)
    {
        // Description
        auto description = Agui::ItemDescriptionBox::Factory(GetRoot(), "description", Agui::RelativePosition(Agui::Dimension(0, 535), Agui::Dimension(0, 5)), 0);
        description->ScaleTo(224, 224);
        description->GetSprite()->color.Edit(100, 255, 100);

        // Context menu
        contextMenu = Agui::ItemContextMenu::Factory(GetRoot(), "contextMenu", Agui::RelativePosition(Agui::Dimension(0, 535), Agui::Dimension(0, 254)), 0);
        contextMenu->GetMenu()->GetLayout()->SetSelfModifySize(false);
        contextMenu->GetMenu()->GetLayout()->ChangeDimensions(224, 224);
        contextMenu->GetMenu()->GetSprite()->color.Edit(100, 255, 100);
        contextMenu->SetShowWithParent(false);

        sections[0].reset(new CharacterSection(*GetRoot(), Agui::RelativePosition(Agui::HorizontalAlignment::RIGHT, Agui::VerticalAlignment::TOP), *description, *contextMenu, *this));
        sections[1].reset(new ReserveSection(*GetRoot(), Agui::RelativePosition(Agui::Dimension(0, 270), Agui::Dimension()), *description, *contextMenu, *this));
        characterSection = sections.begin();
        reserveSection = ++sections.begin();
        (*characterSection)->other = reserveSection->get();
        (*reserveSection)->other = characterSection->get();

        selectedSection = sections.begin();

        boundSubscribers.Add((*characterSection)->gui.eventUpdate, &Inventory::OnSectionUpdated, *this);
        boundSubscribers.Add((*reserveSection)->gui.eventUpdate, &Inventory::OnSectionUpdated, *this);
        boundSubscribers.Subscribe();
    }

    void StatusScreen::Inventory::Update(Entity selected)
    {
        for (auto &loop : sections)
            loop->Update(selected);
    }

    void StatusScreen::Inventory::OnActionPressed(const Input::Action &args)
    {
        switch (args.id)
        {
        case Input::ActionID::NAVIGATE_MENU_LEFT:
        case Input::ActionID::NAVIGATE_MENU_RIGHT:
            if (!contextMenu->IsActive())
            {
                contextMenu->FlipMoving();
                IncSelectedSection();
            }
            break;
        }
    }

    void StatusScreen::Spells::OnSelected()
    {
        gui.Start();
    }

    void StatusScreen::Spells::OnDeselected()
    {
        gui.Stop();
    }

    void StatusScreen::Spells::OnSpellSelected(const AbilityBase &ability)
    {
        if (!IsSelected())
            return;
    }

    StatusScreen::Spells::Spells(Agui::Root &mainRoot) : Page("spells", mainRoot)
    {
        // Description
        auto description = Agui::SpellDescriptionBox::Factory(GetRoot(), "description", Agui::RelativePosition(Agui::Dimension(0, 535), Agui::Dimension(0, 5)), 0);
        description->ScaleTo(224, 224);
        description->GetSprite()->color.Edit(255, 255, 100);

        // Context menu
        auto contextMenu = Agui::SpellContextMenu::Factory(GetRoot(), "contextMenu", Agui::RelativePosition(Agui::Dimension(0, 535), Agui::Dimension(0, 254)), 0);
        contextMenu->GetMenu()->GetLayout()->SetSelfModifySize(false);
        contextMenu->GetMenu()->GetLayout()->ChangeDimensions(224, 224);
        contextMenu->GetMenu()->GetSprite()->color.Edit(255, 255, 100);
        contextMenu->SetShowWithParent(false);

        gui.Init(*GetRoot(), Agui::RelativePosition(Agui::HorizontalAlignment::RIGHT, Agui::VerticalAlignment::TOP), *description, contextMenu);

        AbilityUsing::Instance().eventStarted.Subscribe(&Spells::OnSpellSelected, *this);
    }

    void StatusScreen::Spells::Update(Entity selected)
    {
        gui.SetList(GetCurrentEntities()->FindComponent<Ent::CombatComponent>(selected));
    }

    StatusScreen::Member::Member(Agui::Image &frame) : frame(frame), portrait(*Agui::Image::Factory(&frame, frame.GetName() + "Portrait", Agui::RelativePosition(), 0.1f)), player(Ent::nullEntity)
    {
        portrait.SetMargins(4, 4, 4, 4);
    }

    // Static definitions
    bool StatusScreen::active = false;
    Agui::Root* StatusScreen::root = nullptr;
    StatusScreen::Pages StatusScreen::pages;
    StatusScreen::Pages::iterator StatusScreen::currentPage;
    StatusScreen::Members StatusScreen::members;
    StatusScreen::Members::iterator StatusScreen::selectedMember;
    const StatusScreen::PartySizeT StatusScreen::rowSize = Ent::PlayerParty::maxSize / 2;

    void StatusScreen::OnActionPressed(const Input::Action &args)
    {
        if (!IsActive())
            return;

        auto pageInputFunc = [&](PageID id)
        {
            if (currentPage == pages.find(id))
                Leave();
            else
                Goto(id);
        };

        const Ent::PlayerParty::SizeT halfPartySize = Ent::PlayerParty::maxSize / 2;
        switch (args.id)
        {
        case Input::ActionID::CANCEL:
            Leave();
            break;

        case Input::ActionID::INVENTORY:
            pageInputFunc(PageID::INVENTORY);
            break;
        case Input::ActionID::OPEN_SPELLS:
            pageInputFunc(PageID::SPELLS);
            break;
        case Input::ActionID::STATS:
            pageInputFunc(PageID::STATS);
            break;
        case Input::ActionID::MOVE_LEFT:
            (selectedMember == members.begin()) ? selectedMember = --members.end() : --selectedMember;
            break;
        case Input::ActionID::MOVE_RIGHT:
            (selectedMember == --members.end()) ? selectedMember = members.begin() : ++selectedMember;
        case Input::ActionID::MOVE_UP:
            selectedMember = members.begin() + WrapAdd<unsigned int>(selectedMember - members.begin(), halfPartySize, 0, members.size());
            break;
        case Input::ActionID::MOVE_DOWN:
            selectedMember = members.begin() + WrapSubtract<unsigned int>(selectedMember - members.begin(), halfPartySize, 0, members.size());
            break;
        }

        currentPage->second->OnActionPressed(args);
    }

    void StatusScreen::Activate()
    {
        active = true;
        root->Show();

        // Fetch members
        auto squadLoop = Ent::PlayerParty::Instance().squads.begin();
        auto squadPlayerLoop = squadLoop->begin();
        for (auto memberLoop = members.begin(); memberLoop != members.end(); ++memberLoop)
        {
            memberLoop->player = (*squadPlayerLoop)->GetEntity();
            if (memberLoop->player != Ent::nullEntity)
            {
                memberLoop->portrait.GetSprite()->SetResource(GetCurrentEntities()->FindComponent<Ent::SenseComponent>(memberLoop->player)->sprites.begin()->second->GetMaterial()->GetXVisual()->GetFileName().c_str());
                memberLoop->portrait.Show();
            }
            else
                memberLoop->portrait.Hide();

            ++squadPlayerLoop;
            if (squadPlayerLoop == squadLoop->end())
            {
                ++squadLoop;
                squadPlayerLoop = squadLoop->begin();
            }
        }
    }

    void StatusScreen::Leave()
    {
        active = false;
        root->Hide();
        currentPage->second->Deselect();
    }

    void StatusScreen::Init()
    {
        root = Agui::System::CreateRoot("statusScreen");

        auto image = Agui::Image::Factory(root, "image", Agui::RelativePosition(), 0);
        image->ScaleTo(1024, 768);
        image->GetSprite()->color.Edit(127, 0, 0, 255);

        // Make buttons
        auto arrangerRoot = Agui::ArrangerManager::CreateRoot();
        auto grid = arrangerRoot->Create<Agui::UniformGridLayout>(true, 2, 5.0f);
        grid->SetLeftMargin(5);
        grid->SetTopMargin(5);

        auto button = Agui::PushButton::Factory(image, "statsButton", Agui::RelativePosition(Agui::Dimension(), Agui::Dimension(0, 16)), 0);
        button->GetSprite()->SetResource("buttonSmaller.png");
        button->GetText()->SetString("STATS");
        button->GetText()->color = Agui::colorBlack;
        button->eventClicked.Subscribe(std::bind(&StatusScreen::Goto, StatusScreen::PageID::STATS));
        grid->Add(*button);

        button = Agui::PushButton::Factory(image, "inventoryButton", Agui::RelativePosition(Agui::Dimension(), Agui::Dimension(0, 16)), 0);
        button->GetSprite()->SetResource("buttonSmaller.png");
        button->GetText()->SetString("INVENTORY");
        button->GetText()->color = Agui::colorBlack;
        button->eventClicked.Subscribe(std::bind(&StatusScreen::Goto, StatusScreen::PageID::INVENTORY));
        grid->Add(*button);

        button = Agui::PushButton::Factory(image, "spellsButton", Agui::RelativePosition(Agui::Dimension(), Agui::Dimension(0, 16)), 0);
        button->GetSprite()->SetResource("buttonSmaller.png");
        button->GetText()->SetString("SPELLS");
        button->GetText()->color = Agui::colorBlack;
        button->eventClicked.Subscribe(std::bind(&StatusScreen::Goto, StatusScreen::PageID::SPELLS));
        grid->Add(*button);

        button = Agui::PushButton::Factory(image, "leaveButton", Agui::RelativePosition(Agui::Dimension(), Agui::Dimension(0, 16)), 0);
        button->GetSprite()->SetResource("buttonSmaller.png");
        button->GetText()->SetString("LEAVE");
        button->GetText()->color = Agui::colorBlack;
        button->eventClicked.Subscribe(std::bind(&StatusScreen::Leave));
        grid->Add(*button);

        // Make members
        grid = arrangerRoot->Create<Agui::UniformGridLayout>(true, rowSize, 10.0f);
        grid->SetLeftMargin(16);
        grid->SetTopMargin(80);
        for (Ent::PlayerParty::SizeT loop = 0; loop != Ent::PlayerParty::maxSize; loop++)
        {
            members.push_back(Member(*Agui::Image::Factory(image, ToString(loop), Agui::RelativePosition(), 0)));
            auto &emplaced = members.back();
            emplaced.frame.GetSprite()->SetResource("statusFrame.png");
            grid->Add(emplaced.frame);
        }

        CreatePage<Stats>(PageID::STATS, *root);
        CreatePage<Inventory>(PageID::INVENTORY, *root);
        CreatePage<Spells>(PageID::SPELLS, *root);

        currentPage = pages.begin();
    }

    void StatusScreen::Goto(PageID page)
    {
        if (!active)
            Activate();

        auto found = pages.find(page);
        if (found == pages.end())
            return;

        currentPage->second->Deselect();
        currentPage = found;
        currentPage->second->Select(selectedMember->player);
    }

    bool StatusScreen::IsActive()
    {
        return active;
    }
}