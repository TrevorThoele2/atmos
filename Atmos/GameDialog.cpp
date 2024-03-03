
#include "GameDialog.h"

#include "Environment.h"
#include "MainGame.h"
#include "Battle.h"
#include "StateManager.h"

#include "FontDefines.h"
#include <AGUI\System.h>

namespace Atmos
{
    bool GameDialog::working = false;
    Agui::Root* GameDialog::root = nullptr;
    Agui::Textbox* GameDialog::textbox = nullptr;
    Agui::TextComponent* GameDialog::output = nullptr;
    FrameTimer GameDialog::timer(TimeValue::ValueT(5, 0));
    EventBoundSubscriber GameDialog::inputSub;

    void GameDialog::OnActionPressed(const Input::Action &args)
    {
        if (args.id == Input::ActionID::USE)
            OnUsePressed();
    }

    void GameDialog::OnStateChanged(const StateBase &args)
    {
        if (&args == &mainGameState)// || args == battleState)
            root->Show();
        else
            root->Hide();
    }

    void GameDialog::OnUsePressed()
    {
        if (!IsActive())
            return;

        Leave();
    }

    void GameDialog::Leave()
    {
        working = false;

        inputSub.Unsubscribe();
        textbox->Hide();
    }

    void GameDialog::Init()
    {
        inputSub.Setup(Environment::GetInput()->eventActionPressed, &GameDialog::OnActionPressed);
        StateManager::eventNewState.Subscribe(&GameDialog::OnStateChanged);

        root = Agui::System::CreateRoot("gameDialogRoot");

        textbox = Agui::Textbox::Factory(root, "gameDialog", Agui::RelativePosition(Agui::Dimension(), Agui::Dimension(0, 128), Agui::HorizontalAlignment::MID, Agui::VerticalAlignment::TOP), 0);
        textbox->GetSprite()->color.Edit(255, 100, 255, 100);
        textbox->ScaleTo(256, 128);
        textbox->SetShowWithParent(false);

        output = &textbox->CreateText("output", 1, Agui::RelativePosition(Agui::HorizontalAlignment::MID, Agui::VerticalAlignment::MID), Agui::Text("", Agui::Text::Format(), *Agui::fontSlender, Agui::Color(255, 0, 0, 0))).GetText();
        output->SetAutoCalcTextSize();
    }

    void GameDialog::Work()
    {
        if (!working)
            return;

        if (timer.HasReachedGoal())
            Leave();
    }

    bool GameDialog::Display(const std::string &str)
    {
        working = true;

        inputSub.Subscribe();

        output->SetString(str);
        textbox->Show();

        timer.Start();
        return true;
    }

    bool GameDialog::IsActive()
    {
        return textbox->IsActive();
    }
}