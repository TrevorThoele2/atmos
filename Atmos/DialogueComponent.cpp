
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

        void DialogueComponent::OnOwnerEntitySet()
        {
            SetScriptCallers();
        }

        void DialogueComponent::SetScriptCallers()
        {
            script.GetCaller().Set(GetOwnerEntity());
        }

        DialogueComponent::DialogueComponent(DialogueComponent &&arg) : Component(std::move(arg)), script(std::move(arg.script))
        {}

        DialogueComponent& DialogueComponent::operator=(DialogueComponent &&arg)
        {
            Component::operator=(std::move(arg));
            script = std::move(arg.script);
            return *this;
        }
    }
}