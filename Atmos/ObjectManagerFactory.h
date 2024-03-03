#pragma once

#include "ObjectManager.h"
#include "TypeRegistration.h"

namespace Atmos
{
    class ObjectManagerFactory
    {
    public:
        ObjectManagerFactory(
            TypeRegistration::Group& typeGroup,
            TypeRegistration::Group& infrastructureTypeGroup);

        ObjectManager Create();
    private:
        TypeRegistration::Group* typeGroup;
        TypeRegistration::Group* infrastructureTypeGroup;
    };
}