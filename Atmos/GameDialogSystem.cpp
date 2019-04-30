
#include "GameDialogSystem.h"

#include "ObjectManager.h"
#include "InputSystem.h"

#include "FontDefines.h"
#include <AGUI/System.h>

namespace Atmos
{
    GameDialogSystem::GameDialogSystem(ObjectManager& manager) : ObjectSystem(manager), stopwatch(manager.CreateObject<FrameStopwatch>())
    {}

    GameDialogSystem::GameDialogSystem(const ::Inscription::Table<GameDialogSystem>& table) : INSCRIPTION_TABLE_GET_BASE(ObjectSystem)
    {}

    void GameDialogSystem::Initialize()
    {
        auto input = Manager()->FindSystem<Input::System>();
        inputSubscriber.Setup(input->Get()->eventActionPressed, &GameDialogSystem::OnActionPressed, *this);

        root = Agui::System::CreateRoot("gameDialogRoot");

        textbox = Agui::Textbox::Factory(
            root,
            "gameDialog",
            Agui::RelativePosition(Agui::Dimension(), Agui::Dimension(0, 128), Agui::HorizontalAlignment::MID, Agui::VerticalAlignment::TOP),
            0);
        textbox->GetSprite()->color.Edit(255, 100, 255, 100);
        textbox->ScaleTo(256, 128);
        textbox->SetShowWithParent(false);

        output = &textbox->CreateText(
            "output",
            1,
            Agui::RelativePosition(Agui::HorizontalAlignment::MID, Agui::VerticalAlignment::MID),
            Agui::Text("", Agui::Text::Format(), *Agui::fontSlender, Agui::Color(255, 0, 0, 0))).GetText();
        output->SetAutoCalcTextSize();
    }

    void GameDialogSystem::Work()
    {
        if (!working)
            return;

        if (stopwatch->HasReachedGoal())
            Leave();
    }

    bool GameDialogSystem::Display(const String& str)
    {
        working = true;

        inputSubscriber.Subscribe();

        output->SetString(str);
        textbox->Show();

        stopwatch->Start();
        return true;
    }

    bool GameDialogSystem::IsActive()
    {
        return textbox->IsActive();
    }

    void GameDialogSystem::OnActionPressed(const Input::Action& args)
    {
        if (args.id == Input::ActionID::USE)
            OnUsePressed();
    }

    void GameDialogSystem::OnUsePressed()
    {
        if (!IsActive())
            return;

        Leave();
    }

    void GameDialogSystem::Leave()
    {
        working = false;

        inputSubscriber.Unsubscribe();
        textbox->Hide();
    }
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DEFINE_TABLE(::Atmos::GameDialogSystem)
    {
        INSCRIPTION_INSCRIPTER_CREATE_TABLE;

        INSCRIPTION_TABLE_ADD_BASE(::Atmos::ObjectSystem);

        INSCRIPTION_INSCRIPTER_RETURN_TABLE;
    }

    INSCRIPTION_DEFINE_SIMPLE_CLASS_NAME_RESOLVER(::Atmos::GameDialogSystem, "GameDialogSystem");
}