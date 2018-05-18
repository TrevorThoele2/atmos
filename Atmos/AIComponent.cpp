
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

        AIComponent::AIComponent(AIComponent &&arg) : ai(std::move(arg.ai)), battleAI(std::move(arg.battleAI))
        {}

        AIComponent& AIComponent::operator=(AIComponent &&arg)
        {
            ai = std::move(arg.ai);
            battleAI = std::move(arg.battleAI);
            return *this;
        }
    }
}