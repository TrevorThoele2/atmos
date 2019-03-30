#pragma once

#include "RegistryObjectFactory.h"

#include "CharacterClass.h"

namespace Atmos
{
    template<>
    class ObjectFactory<nCharacterClass> : public RegistryObjectFactory<nCharacterClass>
    {
    public:
        ObjectFactory(ObjectManager& manager);
    };
}