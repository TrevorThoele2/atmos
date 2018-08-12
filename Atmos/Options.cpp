
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
    std::string GetAspectRatioName(const Agui::Resolution::AspectRatio &ratio)
    {
        return ToString(ratio.GetWidth()) + ":" + ToString(ratio.GetHeight());
    }

    std::string GetResolutionName(const Agui::Resolution &res)
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
        auto background = Agui::Image::Factory(GetRoot(), "background", Agui::RelativePosition(Agui::HorizontalAlignment::MID, Agui::VerticalAlignment::MID), 0);
        background->GetSprite()->color.Edit(127, 100, 100, 255);
        Agui::CreateRootResFitter(*background);

        // Fullscreen checkbox
        fullscreenCheckbox = Agui::Checkbox::Factory(GetRoot(), "fullscreen", Agui::RelativePosition(Agui::Dimension(0, 50), Agui::Dimension(0, 30)), 1);
        fullscreenCheckbox->eventCheckedChanged.Subscribe(std::bind(&OptionsScreenGui::OnFullscreenChanged, this, std::placeholders::_1));
        fullscreenCheckbox->SetChecked(!Environment::GetFlag(Environment::Flag::WINDOWED));

        // Fullscreen label
        auto label = Agui::Label::Factory(fullscreenCheckbox, "label", Agui::RelativePosition(Agui::Dimension(), Agui::Dimension(0, -20), Agui::HorizontalAlignment::MID, Agui::VerticalAlignment::TOP), 0);
        label->ModifyText(Agui::Text("FULLSCREEN:", Agui::Text::Format({ Agui::Text::CENTER_HORIZONTAL, Agui::Text::SINGLE_LINE, Agui::Text::CENTER_VERTICAL }), *Agui::fontSlender, Agui::colorBlack));
        label->GetText()->SetAutoCalcTextSize();

        // Master sound fillbar
        masterSoundFillbar = Agui::Fillbar::Factory(GetRoot(), "fillbar", Agui::RelativePosition(Agui::Dimension(0, 50), Agui::Dimension(0, 100)), 1);
        masterSoundFillbar->GetFillerSprite()->color.Edit(255, 0, 255, 0);
        masterSoundFillbar->GetFillerSprite()->SetMargins(1, 1, 1, 1);
        masterSoundFillbar->eventPercentChanged.Subscribe(std::bind(&OptionsScreenGui::OnMasterSoundPercentageChange, this, std::placeholders::_1));

        // Master sound percent label
        masterSoundLabel = Agui::Label::Factory(masterSoundFillbar, "fillbarPercent", Agui::RelativePosition(Agui::Dimension(), Agui::Dimension(0, 20), Agui::HorizontalAlignment::MID, Agui::VerticalAlignment::MID), 0);
        masterSoundLabel->ModifyText(Agui::Text("", Agui::Text::Format({ Agui::Text::CENTER_HORIZONTAL, Agui::Text::SINGLE_LINE, Agui::Text::CENTER_VERTICAL }), *Agui::fontSlender, Agui::colorBlack));
        masterSoundLabel->GetText()->SetAutoCalcTextSize();
        SetMasterSoundLabelString();

        masterSoundFillbar->SetPercent(MasterSoundHandler::GetMasterVolume());

        // Master sound label
        label = Agui::Label::Factory(masterSoundFillbar, "masterSoundLabel", Agui::RelativePosition(Agui::Dimension(), Agui::Dimension(0, -20), Agui::HorizontalAlignment::MID, Agui::VerticalAlignment::TOP), 0);
        label->ModifyText(Agui::Text("MASTER SOUND:", Agui::Text::Format({ Agui::Text::CENTER_HORIZONTAL, Agui::Text::SINGLE_LINE, Agui::Text::CENTER_VERTICAL }), *Agui::fontSlender, Agui::colorBlack));
        label->GetText()->SetAutoCalcTextSize();

        // Resolutions
        // Aspect ratio menubutton
        ratioButton = Agui::MenuButton::Factory(GetRoot(), "aspectRatioButton", Agui::RelativePosition(Agui::Dimension(0, -306), Agui::Dimension(0, 25), Agui::HorizontalAlignment::RIGHT, Agui::VerticalAlignment::TOP), 1);
        ratioButton->GetButton()->GetText()->SetString(GetAspectRatioName(Agui::System::GetCurrentResolution()->GetAspectRatio()));
        ratioButton->GetButton()->GetText()->color.Edit(0, 0, 0);
        ratioButton->GetMenu()->GetSprite()->color.Edit(255, 0, 0, 0);

        // Fill aspect ratio drop down button with aspect ratios
        auto &aspectRatios = Agui::System::GetAspectRatios();
        for (auto loop = aspectRatios.begin(); loop != aspectRatios.end(); loop++)
        {
            auto &name = GetAspectRatioName(loop->second);
            auto &connection = ratioButton->CreateText(name, 1, Agui::Text(name, Agui::Text::Format({ Agui::Text::CENTER_HORIZONTAL, Agui::Text::SINGLE_LINE, Agui::Text::CENTER_VERTICAL }), *Agui::fontButton, Agui::Color()));
            connection.GetText().SetPosition(Agui::RelativePosition(Agui::HorizontalAlignment::MID, Agui::VerticalAlignment::TOP));
            connection.GetText().eventClicked.Subscribe(std::bind(&OptionsScreenGui::OnAspectRatioTextPressed, this, loop->second));
        }

        // Resolution menubutton
        resButton = Agui::MenuButton::Factory(GetRoot(), "resolutionButton", Agui::RelativePosition(Agui::Dimension(0, -25), Agui::Dimension(0, 25), Agui::HorizontalAlignment::RIGHT, Agui::VerticalAlignment::TOP), 1);
        resButton->GetButton()->GetText()->SetString(GetResolutionName(*Agui::System::GetCurrentResolution()));
        resButton->GetButton()->GetText()->color.Edit(0, 0, 0);
        resButton->GetMenu()->GetSprite()->color.Edit(255, 0, 0, 0);

        // Fill dropDownButton with resolutions
        FillResButton(Agui::System::GetCurrentResolution()->GetAspectRatio());

        // Accept button
        auto button = Agui::PushButton::Factory(GetRoot(), "accept", Agui::RelativePosition(Agui::Dimension(0, 60), Agui::Dimension(0, -48), Agui::HorizontalAlignment::LEFT, Agui::VerticalAlignment::BOT), 1);
        button->GetText()->SetString("ACCEPT");
        button->GetText()->color.Edit(0, 0, 0);
        button->eventClicked.Subscribe(std::bind(&OptionsScreenGui::OnAcceptButtonPressed, this));

        // Cancel button
        button = Agui::PushButton::Factory(GetRoot(), "cancel", Agui::RelativePosition(Agui::Dimension(0, -60), Agui::Dimension(0, -48), Agui::HorizontalAlignment::RIGHT, Agui::VerticalAlignment::BOT), 1);
        button->GetText()->SetString("CANCEL");
        button->GetText()->color.Edit(0, 0, 0);
        button->eventClicked.Subscribe(std::bind(&OptionsScreenGui::OnCancelButtonPressed, this));

        // Goto controls button
        button = Agui::PushButton::Factory(GetRoot(), "controls", Agui::RelativePosition(Agui::Dimension(), Agui::Dimension(0, -48), Agui::HorizontalAlignment::MID, Agui::VerticalAlignment::BOT), 1);
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
        resolution.Init(Agui::System::GetCurrentResolution());
    }

    void OptionsScreenGui::Exit()
    {
        Environment::GetIni().Save();
        StateManager::Pop();
    }

    void OptionsScreenGui::OnFullscreenChanged(const Agui::Checkbox &args)
    {
        fullscreen.SetCurrent(args.GetChecked());
    }

    void OptionsScreenGui::OnAspectRatioTextPressed(const Agui::Resolution::AspectRatio &ratio)
    {
        ratioButton->GetButton()->GetText()->SetString(GetAspectRatioName(ratio));
        FillResButton(ratio);
    }

    void OptionsScreenGui::OnResolutionTextPressed(const Agui::Resolution &res)
    {
        resButton->GetButton()->GetText()->SetString(GetResolutionName(res));
        resolution.SetCurrent(&res);
    }

    void OptionsScreenGui::OnMasterSoundPercentageChange(const Agui::Fillbar &args)
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
        Agui::System::SetRes(*resolution.GetCurrent());

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

    void OptionsScreenGui::FillResButton(const Agui::Resolution::AspectRatio &ratio)
    {
        resButton->ClearText();
        auto &container = Agui::System::GetResolutionsForAspectRatio(ratio);
        for (auto loop = container.begin(); loop != container.end(); loop++)
        {
            auto &name = GetResolutionName(**loop);
            auto &connection = resButton->CreateText(name, 1, Agui::Text(name, Agui::Text::Format({ Agui::Text::CENTER_HORIZONTAL, Agui::Text::SINGLE_LINE, Agui::Text::CENTER_VERTICAL }), *Agui::fontButton, Agui::Color()));
            connection.GetText().SetPosition(Agui::RelativePosition(Agui::HorizontalAlignment::MID, Agui::VerticalAlignment::TOP));
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