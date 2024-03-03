
#include "EntitySystem.h"

#include "MainGame.h"
#include "Battle.h"
#include "WorldManager.h"

namespace Atmos
{
    namespace Ent
    {
        SystemBase::ApprovedStates::ApprovedStates(bool mainGame, bool battle) : mainGame(mainGame), battle(battle)
        {}

        bool SystemBase::ApprovedStates::Approval() const
        {
            if (SystemHandler::IgnoreStateApproval())
                return true;

            if (mainGame && mainGameState.IsTop())
                return true;
            else if (battle && battleState.IsTop())
                return true;

            return false;
        }

        void SystemBase::Init()
        {}

        bool SystemBase::IsCurrentStateApproved() const
        {
            return GetApprovedStates().Approval();
        }

        SystemBase::ApprovedStates SystemBase::GetApprovedStates() const
        {
            return ApprovedStates(true, true);
        }

        SystemHandler::SystemHandler() : ignoreStateApproval(false)
        {}

        SystemHandler& SystemHandler::Instance()
        {
            static SystemHandler instance;
            return instance;
        }

        void SystemHandler::Init()
        {
            for (auto &loop : Instance().systems)
                loop->Init();
        }
        
        // If this is set, then systems will ignore their state approval
        void SystemHandler::IgnoreStateApproval(bool set)
        {
            Instance().ignoreStateApproval = set;
        }

        bool SystemHandler::IgnoreStateApproval()
        {
            return Instance().ignoreStateApproval;
        }
    }
}