#pragma once

#include "RegistryObjectFactory.h"

#include "Item.h"

namespace Atmos
{
    template<>
    class ObjectFactory<nItem> : public RegistryObjectFactory<nItem>
    {
    public:
        ObjectFactory(ObjectManager& manager);
    };
}