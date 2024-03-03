
#include "OptionsState.h"

#include "ObjectManager.h"
#include "StateSystem.h"
#include "InitializationFileSystem.h"
#include "WindowSystem.h"
#include "MasterSoundSystem.h"

#include "StringUtility.h"

#include "FontDefines.h"
#include <AGUI/System.h>
#include <AGUI/Image.h>
#include <AGUI/PushButton.h>

namespace Atmos
{
    std::string AspectRatioName(const Agui::Resolution::AspectRatio& ratio)
    {
        return ToString(ratio.GetWidth()) + ":" + ToString(ratio.GetHeight());
    }

    std::string ResolutionName(const Agui::Resolution& resolution)
    {
        auto& size = resolution.GetSize();
        return ToString(size.width) + "x" + ToString(size.height);
    }

    OptionsStateGui::OptionsStateGui(ObjectManager& objectManager) :
        StateGui(objectManager, "options"),
        masterSound(*this), fullscreen(*this), resolution(*this),
        resolutionButton(nullptr), ratioButton(nullptr), fullscreenCheckbox(nullptr),
        masterSoundFillbar(nullptr), masterSoundLabel(nullptr)
    {}

    OptionsStateGui::AttributeBase::AttributeBase(OptionsStateGui& owner)
    {
        owner.attributes.push_back(this);
    }

    OptionsStateGui::AttributeBase::~AttributeBase()
    {}

    void OptionsStateGui::DoInitialize()
    {
        auto root = Root();

        // Background
        auto background = Agui::Image::Factory(
            root,
            "background",
            Agui::RelativePosition(
                Agui::HorizontalAlignment::MID,
                Agui::VerticalAlignment::MID),
            0);
        background->GetSprite()->color.Edit(127, 100, 100, 255);
        Agui::CreateRootResFitter(*background);

        // Fullscreen checkbox
        fullscreenCheckbox = Agui::Checkbox::Factory(
            root,
            "fullscreen",
            Agui::RelativePosition(
                Agui::Dimension(0, 50),
                Agui::Dimension(0, 30)),
            1);
        fullscreenCheckbox->eventCheckedChanged.Subscribe(std::bind(&OptionsStateGui::OnFullscreenChanged, this, std::placeholders::_1));
        fullscreenCheckbox->SetChecked(FindWindowSystem()->Get()->IsFullscreen());

        // Fullscreen label
        auto label = Agui::Label::Factory(
            fullscreenCheckbox,
            "label",
            Agui::RelativePosition(
                Agui::Dimension(),
                Agui::Dimension(0, -20),
                Agui::HorizontalAlignment::MID,
                Agui::VerticalAlignment::TOP),
            0);
        label->ModifyText(
            Agui::Text(
                "FULLSCREEN:",
                Agui::Text::Format({
                    Agui::Text::CENTER_HORIZONTAL,
                    Agui::Text::SINGLE_LINE,
                    Agui::Text::CENTER_VERTICAL }),
                *Agui::fontSlender,
                Agui::colorBlack));
        label->GetText()->SetAutoCalcTextSize();

        // Master sound fillbar
        masterSoundFillbar = Agui::Fillbar::Factory(
            root,
            "fillbar",
            Agui::RelativePosition(
                Agui::Dimension(0, 50),
                Agui::Dimension(0, 100)),
            1);
        masterSoundFillbar->GetFillerSprite()->color.Edit(255, 0, 255, 0);
        masterSoundFillbar->GetFillerSprite()->SetMargins(1, 1, 1, 1);
        masterSoundFillbar->eventPercentChanged.Subscribe(std::bind(&OptionsStateGui::OnMasterSoundPercentageChange, this, std::placeholders::_1));

        // Master sound percent label
        masterSoundLabel = Agui::Label::Factory(
            masterSoundFillbar,
            "fillbarPercent",
            Agui::RelativePosition(
                Agui::Dimension(),
                Agui::Dimension(0, 20),
                Agui::HorizontalAlignment::MID,
                Agui::VerticalAlignment::MID),
            0);
        masterSoundLabel->ModifyText(Agui::Text(
                "",
                Agui::Text::Format({
                    Agui::Text::CENTER_HORIZONTAL,
                    Agui::Text::SINGLE_LINE,
                    Agui::Text::CENTER_VERTICAL }),
                *Agui::fontSlender,
                Agui::colorBlack));
        masterSoundLabel->GetText()->SetAutoCalcTextSize();
        SetMasterSoundLabelString();

        masterSoundFillbar->SetPercent(FindMasterSoundSystem()->GetMasterVolume());

        // Master sound label
        label = Agui::Label::Factory(
            masterSoundFillbar,
            "masterSoundLabel",
            Agui::RelativePosition(
                Agui::Dimension(),
                Agui::Dimension(0, -20),
                Agui::HorizontalAlignment::MID,
                Agui::VerticalAlignment::TOP),
            0);
        label->ModifyText(Agui::Text(
            "MASTER SOUND:",
            Agui::Text::Format({
                Agui::Text::CENTER_HORIZONTAL,
                Agui::Text::SINGLE_LINE,
                Agui::Text::CENTER_VERTICAL }),
            *Agui::fontSlender,
            Agui::colorBlack));
        label->GetText()->SetAutoCalcTextSize();

        // Resolutions
        // Aspect ratio menubutton
        ratioButton = Agui::MenuButton::Factory(
            root,
            "aspectRatioButton",
            Agui::RelativePosition(
                Agui::Dimension(0, -306),
                Agui::Dimension(0, 25),
                Agui::HorizontalAlignment::RIGHT,
                Agui::VerticalAlignment::TOP),
            1);
        ratioButton->GetButton()->GetText()->SetString(AspectRatioName(Agui::System::GetCurrentResolution()->GetAspectRatio()));
        ratioButton->GetButton()->GetText()->color.Edit(0, 0, 0);
        ratioButton->GetMenu()->GetSprite()->color.Edit(255, 0, 0, 0);

        // Fill aspect ratio drop down button with aspect ratios
        auto &aspectRatios = Agui::System::GetAspectRatios();
        for (auto loop = aspectRatios.begin(); loop != aspectRatios.end(); loop++)
        {
            auto &name = AspectRatioName(loop->second);
            auto &connection = ratioButton->CreateText(
                name,
                1,
                Agui::Text(
                    name,
                    Agui::Text::Format({
                        Agui::Text::CENTER_HORIZONTAL,
                        Agui::Text::SINGLE_LINE,
                        Agui::Text::CENTER_VERTICAL }),
                    *Agui::fontButton,
                    Agui::Color()));
            connection.GetText().SetPosition(Agui::RelativePosition(Agui::HorizontalAlignment::MID, Agui::VerticalAlignment::TOP));
            connection.GetText().eventClicked.Subscribe(std::bind(&OptionsStateGui::OnAspectRatioTextPressed, this, loop->second));
        }

        // Resolution menubutton
        resolutionButton = Agui::MenuButton::Factory(
            root,
            "resolutionButton",
            Agui::RelativePosition(
                Agui::Dimension(0, -25),
                Agui::Dimension(0, 25),
                Agui::HorizontalAlignment::RIGHT,
                Agui::VerticalAlignment::TOP),
            1);
        resolutionButton->GetButton()->GetText()->SetString(ResolutionName(*Agui::System::GetCurrentResolution()));
        resolutionButton->GetButton()->GetText()->color.Edit(0, 0, 0);
        resolutionButton->GetMenu()->GetSprite()->color.Edit(255, 0, 0, 0);
        FillResolutionButton(Agui::System::GetCurrentResolution()->GetAspectRatio());

        // Accept button
        auto button = Agui::PushButton::Factory(
            root,
            "accept",
            Agui::RelativePosition(
                Agui::Dimension(0, 60),
                Agui::Dimension(0, -48),
                Agui::HorizontalAlignment::LEFT,
                Agui::VerticalAlignment::BOT),
            1);
        button->GetText()->SetString("ACCEPT");
        button->GetText()->color.Edit(0, 0, 0);
        button->eventClicked.Subscribe(std::bind(&OptionsStateGui::OnAcceptButtonPressed, this));

        // Cancel button
        button = Agui::PushButton::Factory(
            root,
            "cancel",
            Agui::RelativePosition(
                Agui::Dimension(0, -60),
                Agui::Dimension(0, -48),
                Agui::HorizontalAlignment::RIGHT,
                Agui::VerticalAlignment::BOT),
            1);
        button->GetText()->SetString("CANCEL");
        button->GetText()->color.Edit(0, 0, 0);
        button->eventClicked.Subscribe(std::bind(&OptionsStateGui::OnCancelButtonPressed, this));

        // Goto controls button
        button = Agui::PushButton::Factory(
            root,
            "controls",
            Agui::RelativePosition(
                Agui::Dimension(),
                Agui::Dimension(0, -48),
                Agui::HorizontalAlignment::MID,
                Agui::VerticalAlignment::BOT),
            1);
        button->GetText()->SetString("CONTROLS");
        button->GetText()->color.Edit(0, 0, 0);
        button->eventClicked.Subscribe([](const ::Agui::Object&)
        {
            // GO TO CONTROLS
        });
    }

    void OptionsStateGui::DoShow()
    {
        masterSound.Initialize(masterSoundFillbar->GetPercent());
        fullscreen.Initialize(fullscreenCheckbox->GetChecked());
        resolution.Initialize(Agui::System::GetCurrentResolution());
    }

    void OptionsStateGui::Exit()
    {
        FindInitializationFileSystem()->Save();
        FindStateSystem()->Pop();
    }

    void OptionsStateGui::FillResolutionButton(const Agui::Resolution::AspectRatio &ratio)
    {
        resolutionButton->ClearText();
        auto& resolutionList = Agui::System::GetResolutionsForAspectRatio(ratio);
        for (auto& loop : resolutionList)
        {
            auto& name = ResolutionName(*loop);
            auto& connection = resolutionButton->CreateText(
                name,
                1,
                Agui::Text(
                    name,
                    Agui::Text::Format({
                        Agui::Text::CENTER_HORIZONTAL,
                        Agui::Text::SINGLE_LINE,
                        Agui::Text::CENTER_VERTICAL }),
                        *Agui::fontButton,
                        Agui::Color()));
            connection.GetText().SetPosition(Agui::RelativePosition(Agui::HorizontalAlignment::MID, Agui::VerticalAlignment::TOP));
            connection.GetText().eventClicked.Subscribe(std::bind(&OptionsStateGui::OnResolutionTextPressed, this, *loop));
        }
    }

    void OptionsStateGui::SetMasterSoundLabelString()
    {
        auto masterSoundPercent = static_cast<unsigned char>(masterSoundFillbar->GetPercent() * 100);
        masterSoundLabel->GetText()->SetString(ToString(masterSoundPercent) + "%");
    }

    void OptionsStateGui::OnFullscreenChanged(const Agui::Checkbox &args)
    {
        fullscreen.SetCurrent(args.GetChecked());
    }

    void OptionsStateGui::OnAspectRatioTextPressed(const Agui::Resolution::AspectRatio &ratio)
    {
        ratioButton->GetButton()->GetText()->SetString(AspectRatioName(ratio));
        FillResolutionButton(ratio);
    }

    void OptionsStateGui::OnResolutionTextPressed(const Agui::Resolution &res)
    {
        resolutionButton->GetButton()->GetText()->SetString(ResolutionName(res));
        resolution.SetCurrent(&res);
    }

    void OptionsStateGui::OnMasterSoundPercentageChange(const Agui::Fillbar &args)
    {
        masterSound.SetCurrent(args.GetPercent());
        SetMasterSoundLabelString();
        FindMasterSoundSystem()->SetMasterVolume(args.GetPercent());
    }

    void OptionsStateGui::OnAcceptButtonPressed()
    {
        for (auto& loop : attributes)
            loop->Accept();

        // Set resolution depending on what the dropdown says
        Agui::System::SetRes(*resolution.GetCurrent());

        // Set fullscreen depending on what the fullscreen checkbox is
        FindWindowSystem()->Get()->SetFullscreen(fullscreen.GetCurrent());

        Exit();
    }

    void OptionsStateGui::OnCancelButtonPressed()
    {
        for (auto& loop : attributes)
            loop->Decline();

        // Reset resButton
        resolutionButton->GetButton()->GetText()->SetString(ResolutionName(*resolution.GetCurrent()));
        // Reset fullscreen checkbox
        fullscreenCheckbox->SetChecked(FindWindowSystem()->Get()->IsFullscreen());

        // Reset master volume
        FindMasterSoundSystem()->SetMasterVolume(masterSound.GetCurrent());
        masterSoundFillbar->SetPercent(masterSound.GetCurrent());
        SetMasterSoundLabelString();

        Exit();
    }

    StateSystem* OptionsStateGui::FindStateSystem()
    {
        return objectManager->FindSystem<StateSystem>();
    }

    InitializationFileSystem* OptionsStateGui::FindInitializationFileSystem()
    {
        return objectManager->FindSystem<InitializationFileSystem>();
    }

    WindowSystem* OptionsStateGui::FindWindowSystem()
    {
        return objectManager->FindSystem<WindowSystem>();
    }

    MasterSoundSystem* OptionsStateGui::FindMasterSoundSystem()
    {
        return objectManager->FindSystem<MasterSoundSystem>();
    }

    const ObjectTypeName ObjectTraits<OptionsState>::typeName = "OptionsState";
}

namespace Inscription
{
    OBJECT_INSCRIPTER_DEFINE_MEMBERS(::Atmos::OptionsState)
    {

    }
}