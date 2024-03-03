
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
    agui::Root* GameDialog::root = nullptr;
    agui::Textbox* GameDialog::textbox = nullptr;
    agui::TextComponent* GameDialog::output = nullptr;
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

        root = agui::System::CreateRoot("gameDialogRoot");

        textbox = agui::Textbox::Factory(root, "gameDialog", agui::RelativePosition(agui::Dimension(), agui::Dimension(0, 128), agui::HorizontalAlignment::MID, agui::VerticalAlignment::TOP), 0);
        textbox->GetSprite()->color.Edit(255, 100, 255, 100);
        textbox->ScaleTo(256, 128);
        textbox->SetShowWithParent(false);

        output = &textbox->CreateText("output", 1, agui::RelativePosition(agui::HorizontalAlignment::MID, agui::VerticalAlignment::MID), agui::Text("", agui::Text::Format(), *agui::fontSlender, agui::Color(255, 0, 0, 0))).GetText();
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