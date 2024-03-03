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
        AssetSystem(const ::Inscription::Table<AssetSystem>& table);

        Reference FindAsset(const Name& withName) const;

        bool Exists(const Name& withName) const;

        ObjectBatchSizeT Size() const;
    private:
        typedef ObjectBatch<nAsset> Batch;
    private:
        Batch batch;
    private:
        INSCRIPTION_ACCESS;
    };

    template<class T>
    AssetSystem<T>::AssetSystem(ObjectManager& manager) : ObjectSystem(manager)
    {
        batch = manager.Batch<nAsset>();
    }

    template<class T>
    AssetSystem<T>::AssetSystem(const ::Inscription::Table<AssetSystem>& table) :
        INSCRIPTION_TABLE_GET_BASE(ObjectSystem), INSCRIPTION_TABLE_GET_MEM(batch)
    {}

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
    class Inscripter<::Atmos::AssetSystem<T>> : public InscripterBase<::Atmos::AssetSystem<T>>
    {
    public:
        INSCRIPTION_INSCRIPTER_DECLARE_TABLE;
    };

    template<class T>
    typename Inscripter<::Atmos::AssetSystem<T>>::TableT Inscripter<::Atmos::AssetSystem<T>>::CreateTable(Scribe& scribe)
    {
        INSCRIPTION_INSCRIPTER_CREATE_TABLE;

        INSCRIPTION_TABLE_ADD(batch);

        INSCRIPTION_INSCRIPTER_RETURN_TABLE;
    }
}