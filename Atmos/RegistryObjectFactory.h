#pragma once

#include "ObjectFactory.h"

#include "RegistryObject.h"
#include "Name.h"

namespace Atmos
{
    template<class Asset>
    class RegistryObjectFactory : public ObjectFactoryBase
    {
    public:
        typedef RegistryObject Created;
    public:
        RegistryObjectFactory(ObjectManager& manager);

        Created* CreateObject(const Name& name);
    };

    template<class Asset>
    RegistryObjectFactory<Asset>::RegistryObjectFactory(ObjectManager& manager) : ObjectFactoryBase(manager)
    {}

    template<class Asset>
    typename RegistryObjectFactory<Asset>::Created* RegistryObjectFactory<Asset>::CreateObject(const Name& name)
    {
        if (Manager()->FindSystem<AssetSystem>()->Exists(name))
            return nullptr;

        return new Created(name);
    }
}