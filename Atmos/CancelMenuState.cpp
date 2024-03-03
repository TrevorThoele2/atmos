
#include "CancelMenuState.h"

#include "OptionsState.h"
#include "GameState.h"
#include "MainMenuState.h"
#include "StateSystem.h"

#include "ObjectManager.h"
#include "InputSystem.h"

#include <AGUI/PushButton.h>
#include <AGUI/Image.h>

namespace Atmos
{
    CancelMenuStateGui::CancelMenuStateGui(ObjectManager& objectManager) : StateGui(objectManager, "cancelMenu")
    {}

    void CancelMenuStateGui::DoInitialize()
    {
        auto root = Root();

        // Background
        auto image = Agui::Image::Factory(root, "background", Agui::RelativePosition(Agui::Dimension(0.5, 0), Agui::Dimension(0.5, 0)), 0);
        image->GetSprite()->color.Edit(127, 100, 100, 255);
        Agui::CreateRootResFitter(*image);

        // Resume button
        auto button = Agui::PushButton::Factory(
            root,
            "resume",
            Agui::RelativePosition(
                Agui::Dimension(),
                Agui::Dimension(0, -80),
                Agui::HorizontalAlignment::MID,
                Agui::VerticalAlignment::MID),
            1);
        button->GetText()->SetString("RESUME GAME");
        button->GetText()->color.Edit(0, 0, 0);
        button->eventClicked.Subscribe([this](const Agui::Object&)
        {
            objectManager->FindSystem<StateSystem>()->Pop();
        });

        // Save game button
        button = Agui::PushButton::Factory(
            root,
            "saveGame",
            Agui::RelativePosition(
                Agui::HorizontalAlignment::MID,
                Agui::VerticalAlignment::MID),
            1);
        button->GetText()->SetString("SAVE GAME");
        button->GetText()->color.Edit(0, 0, 0);

        // Goto options
        button = Agui::PushButton::Factory(
            root,
            "controls",
            Agui::RelativePosition(
                Agui::Dimension(),
                Agui::Dimension(0, 80),
                Agui::HorizontalAlignment::MID,
                Agui::VerticalAlignment::MID),
            1);
        button->GetText()->SetString("OPTIONS");
        button->GetText()->color.Edit(0, 0, 0);
        button->eventClicked.Subscribe([this](const Agui::Object&)
        {
            objectManager->FindSystem<StateSystem>()->Goto<OptionsState>();
        });

        // Exit game button
        button = Agui::PushButton::Factory(
            root,
            "exit",
            Agui::RelativePosition(
                Agui::Dimension(),
                Agui::Dimension(0, 160),
                Agui::HorizontalAlignment::MID,
                Agui::VerticalAlignment::MID),
            1);
        button->GetText()->SetString("EXIT");
        button->GetText()->color.Edit(0, 0, 0);
        button->eventClicked.Subscribe([this](const Agui::Object&)
        {
            objectManager->FindSystem<StateSystem>()->Goto<MainMenuState>();
        });
    }

    void CancelMenuState::DoInitialize()
    {
        auto inputSystem = Manager()->FindSystem<Input::System>();
        ExecuteWhenTop(inputSystem->Get()->eventKeys.pressed, &CancelMenuState::OnKeyPressed, *this);
    }

    void CancelMenuState::OnKeyPressed(const Input::Key& args)
    {
        if (args.id == Input::KeyID::ESCAPE)
        {
            System()->Pop();
            return;
        }
    }

    const ObjectTypeName ObjectTraits<CancelMenuState>::typeName = "CancelMenuState";
}

namespace Inscription
{
    OBJECT_INSCRIPTER_DEFINE_MEMBERS(::Atmos::CancelMenuState)
    {

    }
}