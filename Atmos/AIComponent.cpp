
#include "AIComponent.h"
#include <Inscription\Inscripter.h>

namespace Atmos
{
    namespace Entity
    {
        AIComponent::AIComponent(ObjectManager& manager, EntityReference owner) :
            Component(manager, owner)
        {
            SetupScripts();
        }

        AIComponent::AIComponent(const ::Inscription::Table<AIComponent>& table) : INSCRIPTION_TABLE_GET_BASE(Component)
        {
            SetupScripts();
        }

        ObjectTypeDescription AIComponent::TypeDescription() const
        {
            return ObjectTraits<AIComponent>::TypeDescription();
        }

        void AIComponent::SetupScripts()
        {
            script->owner = this;
        }
    }

    const ObjectTypeName ObjectTraits<Entity::AIComponent>::typeName = "AIComponent";
}

namespace Inscription
{
    OBJECT_INSCRIPTER_DEFINE_MEMBERS(::Atmos::Entity::AIComponent)
    {
        INSCRIPTION_TABLE_ADD(script);
    }
}