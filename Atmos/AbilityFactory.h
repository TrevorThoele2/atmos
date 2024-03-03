#pragma once

#include "RegistryObjectFactory.h"

#include "Ability.h"

namespace Atmos
{
    template<>
    class ObjectFactory<nAbility> : public RegistryObjectFactory<nAbility>
    {
    public:
        ObjectFactory(ObjectManager& manager);
    };
}