
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
            scribe(upMod);
            scribe(downMod);
            scribe(leftMod);
            scribe(rightMod);
        }

        void MovementComponent::OnOwnerEntitySet()
        {
            SetScriptCallers();
        }

        void MovementComponent::SetScriptCallers()
        {
            upMod.GetCaller().Set(GetOwnerEntity());
            downMod.GetCaller().Set(GetOwnerEntity());
            leftMod.GetCaller().Set(GetOwnerEntity());
            rightMod.GetCaller().Set(GetOwnerEntity());
        }

        MovementComponent::MovementComponent(const MovementComponent &arg) : Component(arg), upMod(arg.upMod), downMod(arg.downMod), leftMod(arg.leftMod), rightMod(arg.rightMod)
        {}

        MovementComponent::MovementComponent(MovementComponent &&arg) : Component(std::move(arg)), upMod(std::move(arg.upMod)), downMod(std::move(arg.downMod)), leftMod(std::move(arg.leftMod)), rightMod(std::move(arg.rightMod))
        {}

        MovementComponent& MovementComponent::operator=(const MovementComponent &arg)
        {
            Component::operator=(arg);
            upMod = arg.upMod;
            downMod = arg.downMod;
            leftMod = arg.leftMod;
            rightMod = arg.rightMod;
            return *this;
        }

        MovementComponent& MovementComponent::operator=(MovementComponent &&arg)
        {
            Component::operator=(std::move(arg));
            upMod = std::move(arg.upMod);
            downMod = std::move(arg.downMod);
            leftMod = std::move(arg.leftMod);
            rightMod = std::move(arg.rightMod);
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