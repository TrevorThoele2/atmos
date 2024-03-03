
#include "AIComponent.h"
#include <Inscription\Inscripter.h>

namespace Atmos
{
    namespace Ent
    {
        nAIComponent::nAIComponent(EntityReference owner) : nEntityComponent(owner)
        {
            SetupScripts();
        }

        nAIComponent::nAIComponent(const ::Inscription::Table<nAIComponent>& table) : INSCRIPTION_TABLE_GET_BASE(nEntityComponent)
        {
            SetupScripts();
        }

        ObjectTypeDescription nAIComponent::TypeDescription() const
        {
            return ObjectTraits<nAIComponent>::TypeDescription();
        }

        void nAIComponent::SetupScripts()
        {
            script->owner = this;
        }
    }

    const ObjectTypeName ObjectTraits<Ent::nAIComponent>::typeName = "AIComponent";
}

namespace Inscription
{
    DEFINE_OBJECT_INSCRIPTER_MEMBERS(::Atmos::Ent::nAIComponent)
    {
        INSCRIPTION_TABLE_ADD(script);
    }
}