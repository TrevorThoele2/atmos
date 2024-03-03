
#include "AvatarComponent.h"
#include <Inscription\Scribe.h>

namespace Atmos
{
    namespace Ent
    {
        nAvatarComponent::nAvatarComponent(EntityReference owner) : nEntityComponent(owner), currency(0)
        {}

        nAvatarComponent::nAvatarComponent(const ::Inscription::Table<nAvatarComponent>& table) : INSCRIPTION_TABLE_GET_BASE(nEntityComponent)
        {}

        ObjectTypeDescription nAvatarComponent::TypeDescription() const
        {
            return ObjectTraits<nAvatarComponent>::TypeDescription();
        }
    }

    const ObjectTypeName ObjectTraits<Ent::nAvatarComponent>::typeName = "AvatarComponent";
}

namespace Inscription
{
    DEFINE_OBJECT_INSCRIPTER_MEMBERS(::Atmos::Ent::nAvatarComponent)
    {
        INSCRIPTION_TABLE_ADD(currency);
    }
}