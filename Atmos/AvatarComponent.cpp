
#include "AvatarComponent.h"
#include <Inscription\Scribe.h>

namespace Atmos::Entity
{
    AvatarComponent::AvatarComponent(ObjectManager& manager, EntityReference owner) :
        Component(manager, owner), currency(0)
    {}

    AvatarComponent::AvatarComponent(const ::Inscription::BinaryTableData<AvatarComponent>& data) :
        Component(std::get<0>(data.bases))
    {}

    ObjectTypeDescription AvatarComponent::TypeDescription() const
    {
        return ObjectTraits<AvatarComponent>::TypeDescription();
    }
}

namespace Atmos
{
    const ObjectTypeName ObjectTraits<Entity::AvatarComponent>::typeName = "AvatarComponent";
}