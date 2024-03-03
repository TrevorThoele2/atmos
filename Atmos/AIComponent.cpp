
#include "AIComponent.h"
#include <Inscription\Inscripter.h>

namespace Atmos
{
    namespace Ent
    {
        INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(AIComponent)
        {
            scribe(ai);
            scribe(battleAI);
        }

        void AIComponent::OnOwnerEntitySet()
        {
            SetScriptCallers();
        }

        void AIComponent::SetScriptCallers()
        {
            ai.GetCaller().Set(GetOwnerEntity());
            battleAI.GetCaller().Set(GetOwnerEntity());
        }

        AIComponent::AIComponent()
        {}

        AIComponent::AIComponent(AIComponent &&arg) : Component(std::move(arg)), ai(std::move(arg.ai)), battleAI(std::move(arg.battleAI))
        {}

        AIComponent& AIComponent::operator=(AIComponent &&arg)
        {
            Component::operator=(std::move(arg));
            ai = std::move(arg.ai);
            battleAI = std::move(arg.battleAI);
            return *this;
        }
    }
}