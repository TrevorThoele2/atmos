
#include "EntityComponentBase.h"

namespace Atmos
{
    namespace Ent
    {
        void ComponentBase::SetOwnerEntity(Entity set)
        {
            ownerEntity = set;
            OnOwnerEntitySet();
        }

        ComponentBase::ComponentBase() : ownerEntity(nullEntity)
        {}

        ComponentBase::ComponentBase(const ComponentBase &arg) : ownerEntity(nullEntity)
        {}

        ComponentBase& ComponentBase::operator=(const ComponentBase &arg)
        {
            return *this;
        }

        ComponentBase::ComponentBase(ComponentBase &&arg) : ownerEntity(arg.ownerEntity)
        {}

        ComponentBase& ComponentBase::operator=(ComponentBase &&arg)
        {
            ownerEntity = arg.ownerEntity;
            return *this;
        }

        Entity ComponentBase::GetOwnerEntity() const
        {
            return ownerEntity;
        }
    }
}