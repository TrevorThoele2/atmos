#pragma once

#include "ObjectSystem.h"
#include "ObjectManager.h"
#include "ObjectReference.h"
#include "ObjectBatch.h"

#include "Asset.h"

#include "Name.h"

namespace Atmos
{
    template<class T>
    class AssetSystem : public ObjectSystem
    {
    public:
        typedef TypedObjectReference<T> Reference;
    public:
        AssetSystem(ObjectManager& manager);

        Reference FindAsset(const Name& withName) const;

        bool Exists(const Name& withName) const;

        ObjectBatchSizeT Size() const;
    private:
        typedef ObjectBatch<Asset> Batch;
    private:
        Batch batch;
    private:
        INSCRIPTION_ACCESS;
    };

    template<class T>
    AssetSystem<T>::AssetSystem(ObjectManager& manager) : ObjectSystem(manager)
    {
        batch = manager.Batch<Asset>();
    }

    template<class T>
    typename AssetSystem<T>::Reference AssetSystem<T>::FindAsset(const Name& withName) const
    {
        for (auto& loop : batch)
            if (loop->name == withName)
                return loop.Downcast<T>();

        return Reference();
    }

    template<class T>
    bool AssetSystem<T>::Exists(const Name& withName) const
    {
        return FindAsset(withName).IsOccupied();
    }

    template<class T>
    ObjectBatchSizeT AssetSystem<T>::Size() const
    {
        return batch.Size();
    }

    template<class T>
    struct ObjectSystemTraits<AssetSystem<T>>
    {
        static const ObjectSystemPriority priority = 0;
    };
}

namespace Inscription
{
    template<class T>
    class Scribe<::Atmos::AssetSystem<T>, BinaryArchive> :
        public ObjectSystemScribe<::Atmos::AssetSystem<T>, BinaryArchive>
    {
    private:
        using BaseT = typename ObjectSystemScribe<::Atmos::AssetSystem<T>, BinaryArchive>;
    public:
        using ObjectT = typename BaseT::ObjectT;
        using ArchiveT = typename BaseT::ArchiveT;
    public:
        static void Scriven(ObjectT& object, ArchiveT& archive)
        {
            BaseScriven<::Atmos::ObjectSystem>(object, archive);
            archive(object.batch);
        }
    };
}