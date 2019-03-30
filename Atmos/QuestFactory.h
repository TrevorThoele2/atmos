#pragma once

#include "RegistryObjectFactory.h"

#include "Quest.h"

namespace Atmos
{
    template<>
    class ObjectFactory<nQuest> : public RegistryObjectFactory<nQuest>
    {
    public:
        ObjectFactory(ObjectManager& manager);
    };
}