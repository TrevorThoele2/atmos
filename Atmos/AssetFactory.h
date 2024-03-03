#pragma once

#include "ObjectFactory.h"

#include "AssetSystem.h"

#include "Name.h"
#include "FilePath.h"

namespace Atmos
{
    template<class Asset>
    class AssetFactory : public ObjectFactoryBase
    {
    public:
        typedef Asset Created;
    public:
        AssetFactory(ObjectManager& manager);

        template<class DataT>
        Created* CreateObject(const Name& name, std::unique_ptr<DataT>&& data);
    };

    template<class Asset>
    AssetFactory<Asset>::AssetFactory(ObjectManager& manager) : ObjectFactoryBase(manager)
    {}

    template<class Asset>
    template<class DataT>
    typename AssetFactory<Asset>::Created* AssetFactory<Asset>::CreateObject(const Name& name, std::unique_ptr<DataT>&& data)
    {
        if (Manager()->FindSystem<AssetSystem>()->Exists(name))
            return nullptr;

        return new Created(name, std::move(data));
    }

    template<class Asset>
    class FileAssetFactory : public ObjectFactoryBase
    {
    public:
        typedef Asset Created;
    public:
        FileAssetFactory(ObjectManager& manager);

        Created* CreateObject(const FileName& fileName);
    };

    template<class Asset>
    FileAssetFactory<Asset>::FileAssetFactory(ObjectManager& manager) : ObjectFactoryBase(manager)
    {}

    template<class Asset>
    typename FileAssetFactory<Asset>::Created* FileAssetFactory<Asset>::CreateObject(const FileName& fileName)
    {
        if (Manager()->FindSystem<AssetSystem>()->Exists(fileName.GetValue()))
            return nullptr;

        return new Created(fileName);
    }
}