#pragma once

#include "RegistryObjectFactory.h"

#include "Spell.h"

namespace Atmos
{
    template<>
    class ObjectFactory<nSpell> : public RegistryObjectFactory<nSpell>
    {
    public:
        ObjectFactory(ObjectManager& manager);
    };
}