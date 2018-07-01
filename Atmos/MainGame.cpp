
#include "MainGame.h"

#include "Input.h"
#include "Logic.h"
#include "Speech.h"
#include "StatusScreen.h"
#include "GameDialog.h"
#include "WorldManager.h"
#include "CancelMenu.h"
#include "ScriptController.h"
#include "ScriptLocator.h"
#include "Camera.h"
#include "Environment.h"

#include "AvatarSystem.h"
#include "EntityAISystem.h"
#include "EntityPositionSystem.h"

#include "FontDefines.h"

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
            StatusScreen::OnActionPressed(args);
            return;
        }

        switch (args.id)
        {
        case Input::ActionID::USE:
            UseOnClick();
            break;
        case Input::ActionID::INVENTORY:
            if (CanNewMenu())
                StatusScreen::Goto(StatusScreen::PageID::INVENTORY);
            break;
        case Input::ActionID::OPEN_SPELLS:
            if (CanNewMenu())
                StatusScreen::Goto(StatusScreen::PageID::SPELLS);
            break;
        case Input::ActionID::STATS:
            if (CanNewMenu())
                StatusScreen::Goto(StatusScreen::PageID::STATS);
            break;
        }
    }

    void MainGame::UseOnClick()
    {
        /*
        // Activate tile in front of the main player
        if (WorldManager::GetCurrentField()->tiles.Activate(mainPlayer->GetAt<MainPlayer::PositionComponent>()->GetPositionInFront(), GameAction::ActivationType::USE))
            return;

        if (!CanNewMenu())
            return;

        // Attempt to talk to the character in front
        auto tile = WorldManager::GetCurrentField()->tiles.Find(MainPlayer::Instance()->GetPosInFront());
        if (!tile || !tile->GetCharacterHere())
            return;

        tile->GetCharacterHere()->TalkTo();
        */
    }

    void MainGame::InitImpl()
    {
        SubscribeEvent(Environment::GetInput()->eventKeys.pressed, &MainGame::OnKeyPressed, *this);
        SubscribeEvent(Environment::GetInput()->eventActionPressed, &MainGame::OnActionPressed, *this);

        StatusScreen::Init();
    }

    void MainGame::WorkImpl()
    {
        GetCurrentScheduler()->Work();
        GetCurrentOrphanScripts()->Work();

        Ent::AISystem::Work();
        Ent::PositionSystem::Work();
        ScriptController::Work();
        ScriptLocatorManager::Work();
        WorldManager::Work();

        GameDialog::Work();
        Speech::Handler::Work();
        Camera::Work();
    }

    bool MainGame::AnyTertiaryOpen() const
    {
        return GameDialog::IsActive() || Speech::Handler::IsWorking() || StatusScreen::IsActive();
    }

    bool MainGame::CanNewMenu() const
    {
        return IsTop() && !AnyTertiaryOpen();
    }

    MainGame mainGameState;
}