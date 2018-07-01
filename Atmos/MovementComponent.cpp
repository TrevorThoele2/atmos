
#include "MovementComponent.h"

#include "Entity.h"
#include "DialogueComponent.h"
#include "BattleComponent.h"

#include "CurrentField.h"

#include <Inscription/Inscripter.h>

namespace Atmos
{
    namespace Ent
    {
        INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(MovementComponent)
        {
            scribe(movementMod);
            scribe(changeDirectionMod);
        }

        void MovementComponent::OnOwnerEntitySet()
        {
            SetScriptCallers();
        }

        void MovementComponent::SetScriptCallers()
        {
            movementMod.GetCaller().Set(GetOwnerEntity());
            changeDirectionMod.GetCaller().Set(GetOwnerEntity());
        }

        MovementComponent::MovementComponent(const MovementComponent &arg) : Component(arg), movementMod(arg.movementMod), changeDirectionMod(arg.changeDirectionMod)
        {}

        MovementComponent::MovementComponent(MovementComponent &&arg) : Component(std::move(arg)), movementMod(std::move(arg.movementMod)), changeDirectionMod(std::move(arg.changeDirectionMod))
        {}

        MovementComponent& MovementComponent::operator=(const MovementComponent &arg)
        {
            Component::operator=(arg);
            movementMod = arg.movementMod;
            changeDirectionMod = arg.changeDirectionMod;
            return *this;
        }

        MovementComponent& MovementComponent::operator=(MovementComponent &&arg)
        {
            Component::operator=(std::move(arg));
            movementMod = std::move(arg.movementMod);
            changeDirectionMod = std::move(arg.changeDirectionMod);
            return *this;
        }

        bool MovementComponent::IsMoving() const
        {
            return false;
        }

        bool MovementComponent::CanMove() const
        {
            // Check dialogue
            {
                auto dialogue = GetCurrentEntities()->FindComponent<DialogueComponent>(GetOwnerEntity());
                if (dialogue && dialogue->script.IsExecuting())
                    return false;
            }

            // Check battle
            {
                auto battle = GetCurrentEntities()->FindComponent<BattleComponent>(GetOwnerEntity());
                if (battle && battle->turnEnded)
                    return false;
            }

            return !IsMoving();
        }
    }
}