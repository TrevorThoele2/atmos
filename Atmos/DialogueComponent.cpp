
#include "DialogueComponent.h"
#include <Inscription\Inscripter.h>

namespace Atmos
{
    namespace Ent
    {
        INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(DialogueComponent)
        {
            scribe(script);
        }

        DialogueComponent::DialogueComponent(DialogueComponent &&arg) : script(std::move(arg.script))
        {}

        DialogueComponent& DialogueComponent::operator=(DialogueComponent &&arg)
        {
            script = std::move(arg.script);
            return *this;
        }
    }
}