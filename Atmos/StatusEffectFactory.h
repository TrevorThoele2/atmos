#pragma once

#include "RegistryObjectFactory.h"

#include "StatusEffect.h"

namespace Atmos
{
    template<>
    class ObjectFactory<nStatusEffect> : public RegistryObjectFactory<nStatusEffect>
    {
    public:
        ObjectFactory(ObjectManager& manager);
    };
}