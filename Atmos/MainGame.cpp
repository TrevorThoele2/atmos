
#include "MainGame.h"

#include "SpeechController.h"
#include "WorldManager.h"
#include "CurrentField.h"
#include "GameEnvironment.h"

#include "CancelMenu.h"
#include "StatusScreen.h"
#include "GameDialog.h"

namespace Atmos
{
    void MainGameGui::InitImpl()
    {
        GameDialog::Init();
    }

    MainGameGui::MainGameGui() : StateGui("mainGame")
    {}

    void MainGame::OnKeyPressed(const Input::Key &args)
    {
        if (args.id == Input::KeyID::ESCAPE)
        {
            if (CanNewMenu())
                cancelMenuState.Goto();

            return;
        }
    }

    void MainGame::OnActionPressed(const Input::Action &args)
    {
        if (!CanNewMenu())
        {
            //StatusScreen::OnActionPressed(args);
            return;
        }

        switch (args.id)
        {
        case Input::ActionID::INVENTORY:
            //if (CanNewMenu())
                //StatusScreen::Goto(StatusScreen::PageID::INVENTORY);
            break;
        case Input::ActionID::OPEN_SPELLS:
            //if (CanNewMenu())
                //StatusScreen::Goto(StatusScreen::PageID::SPELLS);
            break;
        case Input::ActionID::STATS:
            //if (CanNewMenu())
                //StatusScreen::Goto(StatusScreen::PageID::STATS);
            break;
        }
    }

    void MainGame::InitImpl()
    {
        SubscribeEvent(Environment::GetInput()->eventKeys.pressed, &MainGame::OnKeyPressed, *this);
        SubscribeEvent(Environment::GetInput()->eventActionPressed, &MainGame::OnActionPressed, *this);

        //StatusScreen::Init();
    }

    void MainGame::WorkImpl()
    {
        GameDialog::Work();
        GameEnvironment::GetCamera().Work();
    }

    void MainGame::OnFocusedImpl()
    {}

    bool MainGame::AnyTertiaryOpen() const
    {
        auto objectManager = GetLocalObjectManager();
        bool isSpeechWorking = objectManager && objectManager->FindSystem<Speech::Controller>()->IsActive();
        return GameDialog::IsActive() || isSpeechWorking; //|| StatusScreen::IsActive();
    }

    bool MainGame::CanNewMenu() const
    {
        return IsTop() && !AnyTertiaryOpen();
    }

    MainGame mainGameState;
}