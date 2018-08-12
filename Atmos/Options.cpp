
#include "Options.h"

#include "Environment.h"
#include "MasterSoundHandler.h"
#include "StateManager.h"
#include "Controls.h"
#include "StringUtility.h"

#include "FontDefines.h"
#include <AGUI\System.h>
#include <AGUI\Image.h>
#include <AGUI\PushButton.h>

namespace Atmos
{
    std::string GetAspectRatioName(const agui::Resolution::AspectRatio &ratio)
    {
        return ToString(ratio.GetWidth()) + ":" + ToString(ratio.GetHeight());
    }

    std::string GetResolutionName(const agui::Resolution &res)
    {
        auto &size = res.GetSize();
        return ToString(size.width) + "x" + ToString(size.height);
    }

    OptionsScreenGui::AttributeBase::AttributeBase()
    {
        optionsScreen.gui.attributes.push_back(this);
    }

    void OptionsScreenGui::InitImpl()
    {
        // Background
        auto background = agui::Image::Factory(GetRoot(), "background", agui::RelativePosition(agui::HorizontalAlignment::MID, agui::VerticalAlignment::MID), 0);
        background->GetSprite()->color.Edit(127, 100, 100, 255);
        agui::CreateRootResFitter(*background);

        // Fullscreen checkbox
        fullscreenCheckbox = agui::Checkbox::Factory(GetRoot(), "fullscreen", agui::RelativePosition(agui::Dimension(0, 50), agui::Dimension(0, 30)), 1);
        fullscreenCheckbox->eventCheckedChanged.Subscribe(std::bind(&OptionsScreenGui::OnFullscreenChanged, this, std::placeholders::_1));
        fullscreenCheckbox->SetChecked(!Environment::GetFlag(Environment::Flag::WINDOWED));

        // Fullscreen label
        auto label = agui::Label::Factory(fullscreenCheckbox, "label", agui::RelativePosition(agui::Dimension(), agui::Dimension(0, -20), agui::HorizontalAlignment::MID, agui::VerticalAlignment::TOP), 0);
        label->ModifyText(agui::Text("FULLSCREEN:", agui::Text::Format({ agui::Text::CENTER_HORIZONTAL, agui::Text::SINGLE_LINE, agui::Text::CENTER_VERTICAL }), *agui::fontSlender, agui::colorBlack));
        label->GetText()->SetAutoCalcTextSize();

        // Master sound fillbar
        masterSoundFillbar = agui::Fillbar::Factory(GetRoot(), "fillbar", agui::RelativePosition(agui::Dimension(0, 50), agui::Dimension(0, 100)), 1);
        masterSoundFillbar->GetFillerSprite()->color.Edit(255, 0, 255, 0);
        masterSoundFillbar->GetFillerSprite()->SetMargins(1, 1, 1, 1);
        masterSoundFillbar->eventPercentChanged.Subscribe(std::bind(&OptionsScreenGui::OnMasterSoundPercentageChange, this, std::placeholders::_1));

        // Master sound percent label
        masterSoundLabel = agui::Label::Factory(masterSoundFillbar, "fillbarPercent", agui::RelativePosition(agui::Dimension(), agui::Dimension(0, 20), agui::HorizontalAlignment::MID, agui::VerticalAlignment::MID), 0);
        masterSoundLabel->ModifyText(agui::Text("", agui::Text::Format({ agui::Text::CENTER_HORIZONTAL, agui::Text::SINGLE_LINE, agui::Text::CENTER_VERTICAL }), *agui::fontSlender, agui::colorBlack));
        masterSoundLabel->GetText()->SetAutoCalcTextSize();
        SetMasterSoundLabelString();

        masterSoundFillbar->SetPercent(MasterSoundHandler::GetMasterVolume());

        // Master sound label
        label = agui::Label::Factory(masterSoundFillbar, "masterSoundLabel", agui::RelativePosition(agui::Dimension(), agui::Dimension(0, -20), agui::HorizontalAlignment::MID, agui::VerticalAlignment::TOP), 0);
        label->ModifyText(agui::Text("MASTER SOUND:", agui::Text::Format({ agui::Text::CENTER_HORIZONTAL, agui::Text::SINGLE_LINE, agui::Text::CENTER_VERTICAL }), *agui::fontSlender, agui::colorBlack));
        label->GetText()->SetAutoCalcTextSize();

        // Resolutions
        // Aspect ratio menubutton
        ratioButton = agui::MenuButton::Factory(GetRoot(), "aspectRatioButton", agui::RelativePosition(agui::Dimension(0, -306), agui::Dimension(0, 25), agui::HorizontalAlignment::RIGHT, agui::VerticalAlignment::TOP), 1);
        ratioButton->GetButton()->GetText()->SetString(GetAspectRatioName(agui::System::GetCurrentResolution()->GetAspectRatio()));
        ratioButton->GetButton()->GetText()->color.Edit(0, 0, 0);
        ratioButton->GetMenu()->GetSprite()->color.Edit(255, 0, 0, 0);

        // Fill aspect ratio drop down button with aspect ratios
        auto &aspectRatios = agui::System::GetAspectRatios();
        for (auto loop = aspectRatios.begin(); loop != aspectRatios.end(); loop++)
        {
            auto &name = GetAspectRatioName(loop->second);
            auto &connection = ratioButton->CreateText(name, 1, agui::Text(name, agui::Text::Format({ agui::Text::CENTER_HORIZONTAL, agui::Text::SINGLE_LINE, agui::Text::CENTER_VERTICAL }), *agui::fontButton, agui::Color()));
            connection.GetText().SetPosition(agui::RelativePosition(agui::HorizontalAlignment::MID, agui::VerticalAlignment::TOP));
            connection.GetText().eventClicked.Subscribe(std::bind(&OptionsScreenGui::OnAspectRatioTextPressed, this, loop->second));
        }

        // Resolution menubutton
        resButton = agui::MenuButton::Factory(GetRoot(), "resolutionButton", agui::RelativePosition(agui::Dimension(0, -25), agui::Dimension(0, 25), agui::HorizontalAlignment::RIGHT, agui::VerticalAlignment::TOP), 1);
        resButton->GetButton()->GetText()->SetString(GetResolutionName(*agui::System::GetCurrentResolution()));
        resButton->GetButton()->GetText()->color.Edit(0, 0, 0);
        resButton->GetMenu()->GetSprite()->color.Edit(255, 0, 0, 0);

        // Fill dropDownButton with resolutions
        FillResButton(agui::System::GetCurrentResolution()->GetAspectRatio());

        // Accept button
        auto button = agui::PushButton::Factory(GetRoot(), "accept", agui::RelativePosition(agui::Dimension(0, 60), agui::Dimension(0, -48), agui::HorizontalAlignment::LEFT, agui::VerticalAlignment::BOT), 1);
        button->GetText()->SetString("ACCEPT");
        button->GetText()->color.Edit(0, 0, 0);
        button->eventClicked.Subscribe(std::bind(&OptionsScreenGui::OnAcceptButtonPressed, this));

        // Cancel button
        button = agui::PushButton::Factory(GetRoot(), "cancel", agui::RelativePosition(agui::Dimension(0, -60), agui::Dimension(0, -48), agui::HorizontalAlignment::RIGHT, agui::VerticalAlignment::BOT), 1);
        button->GetText()->SetString("CANCEL");
        button->GetText()->color.Edit(0, 0, 0);
        button->eventClicked.Subscribe(std::bind(&OptionsScreenGui::OnCancelButtonPressed, this));

        // Goto controls button
        button = agui::PushButton::Factory(GetRoot(), "controls", agui::RelativePosition(agui::Dimension(), agui::Dimension(0, -48), agui::HorizontalAlignment::MID, agui::VerticalAlignment::BOT), 1);
        button->GetText()->SetString("CONTROLS");
        button->GetText()->color.Edit(0, 0, 0);
        button->eventClicked.Subscribe(std::bind(&ControlsScreen::Goto, &controlsScreenState));
    }

    void OptionsScreenGui::ShowImpl()
    {
        // Master sound
        masterSound.Init(masterSoundFillbar->GetPercent());
        // Fullscreen
        fullscreen.Init(fullscreenCheckbox->GetChecked());
        // Resolution attribute 
        resolution.Init(agui::System::GetCurrentResolution());
    }

    void OptionsScreenGui::Exit()
    {
        Environment::GetIni().Save();
        StateManager::Pop();
    }

    void OptionsScreenGui::OnFullscreenChanged(const agui::Checkbox &args)
    {
        fullscreen.SetCurrent(args.GetChecked());
    }

    void OptionsScreenGui::OnAspectRatioTextPressed(const agui::Resolution::AspectRatio &ratio)
    {
        ratioButton->GetButton()->GetText()->SetString(GetAspectRatioName(ratio));
        FillResButton(ratio);
    }

    void OptionsScreenGui::OnResolutionTextPressed(const agui::Resolution &res)
    {
        resButton->GetButton()->GetText()->SetString(GetResolutionName(res));
        resolution.SetCurrent(&res);
    }

    void OptionsScreenGui::OnMasterSoundPercentageChange(const agui::Fillbar &args)
    {
        masterSound.SetCurrent(args.GetPercent());
        SetMasterSoundLabelString();
        MasterSoundHandler::SetMasterVolume(args.GetPercent());
    }

    void OptionsScreenGui::OnAcceptButtonPressed()
    {
        for (auto loop = attributes.begin(); loop != attributes.end(); loop++)
            (*loop)->Accept();

        // Set resolution depending on what the dropdown says
        agui::System::SetRes(*resolution.GetCurrent());

        // Set fullscreen depending on what the fullscreen checkbox is
        Environment::SetFullscreen(fullscreen.GetCurrent());

        Exit();
    }

    void OptionsScreenGui::OnCancelButtonPressed()
    {
        for (auto loop = attributes.begin(); loop != attributes.end(); loop++)
            (*loop)->Decline();

        // Reset resButton
        resButton->GetButton()->GetText()->SetString(GetResolutionName(*resolution.GetCurrent()));
        // Reset fullscreen checkbox
        fullscreenCheckbox->SetChecked(!Environment::Instance().GetFlag(Environment::Flag::WINDOWED));

        // Reset master volume
        MasterSoundHandler::SetMasterVolume(masterSound.GetCurrent());
        masterSoundFillbar->SetPercent(masterSound.GetCurrent());
        SetMasterSoundLabelString();

        Exit();
    }

    void OptionsScreenGui::FillResButton(const agui::Resolution::AspectRatio &ratio)
    {
        resButton->ClearText();
        auto &container = agui::System::GetResolutionsForAspectRatio(ratio);
        for (auto loop = container.begin(); loop != container.end(); loop++)
        {
            auto &name = GetResolutionName(**loop);
            auto &connection = resButton->CreateText(name, 1, agui::Text(name, agui::Text::Format({ agui::Text::CENTER_HORIZONTAL, agui::Text::SINGLE_LINE, agui::Text::CENTER_VERTICAL }), *agui::fontButton, agui::Color()));
            connection.GetText().SetPosition(agui::RelativePosition(agui::HorizontalAlignment::MID, agui::VerticalAlignment::TOP));
            connection.GetText().eventClicked.Subscribe(std::bind(&OptionsScreenGui::OnResolutionTextPressed, this, **loop));
        }
    }

    void OptionsScreenGui::SetMasterSoundLabelString()
    {
        masterSoundLabel->GetText()->SetString(ToString(static_cast<unsigned char>(masterSoundFillbar->GetPercent() * 100)) + "%");
    }

    OptionsScreenGui::OptionsScreenGui() : StateGui("options"), resButton(nullptr), ratioButton(nullptr), fullscreenCheckbox(nullptr), masterSoundFillbar(nullptr), masterSoundLabel(nullptr)
    {}

    OptionsScreen optionsScreen;
}