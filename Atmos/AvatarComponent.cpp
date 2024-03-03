
#include "AvatarComponent.h"
#include <Inscription\Scribe.h>

namespace Atmos
{
    namespace Entity
    {
        AvatarComponent::AvatarComponent(ObjectManager& manager, EntityReference owner) :
            Component(manager, owner), currency(0)
        {}

        AvatarComponent::AvatarComponent(const ::Inscription::Table<AvatarComponent>& table) : INSCRIPTION_TABLE_GET_BASE(Component)
        {}

        ObjectTypeDescription AvatarComponent::TypeDescription() const
        {
            return ObjectTraits<AvatarComponent>::TypeDescription();
        }
    }

    const ObjectTypeName ObjectTraits<Entity::AvatarComponent>::typeName = "AvatarComponent";
}

namespace Inscription
{
    OBJECT_INSCRIPTER_DEFINE_MEMBERS(::Atmos::Entity::AvatarComponent)
    {
        INSCRIPTION_TABLE_ADD(currency);
    }
}