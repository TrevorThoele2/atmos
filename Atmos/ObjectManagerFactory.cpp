#include "ObjectManagerFactory.h"

namespace Atmos
{
    ObjectManagerFactory::ObjectManagerFactory(
        TypeRegistration::Group& typeGroup,
        TypeRegistration::Group& infrastructureTypeGroup) :

        typeGroup(&typeGroup), infrastructureTypeGroup(&infrastructureTypeGroup)
    {}

    ObjectManager ObjectManagerFactory::Create()
    {
        ObjectManager created;
        typeGroup->PushTo(created);
        infrastructureTypeGroup->PushTo(created);
        return created;
    }
}