#pragma once

#include "ObjectSystem.h"
#include "ObjectManager.h"
#include "ObjectReference.h"
#include "ObjectBatch.h"

#include "RegistryObject.h"

#include "Name.h"

namespace Atmos
{
    template<class T>
    class RegistryObjectSystem : public ObjectSystem
    {
    public:
        typedef TypedObjectReference<T> Reference;
    public:
        RegistryObjectSystem(ObjectManager& manager);
        RegistryObjectSystem(const ::Inscription::Table<RegistryObjectSystem>& table);

        Reference FindRegistryObject(const Name& withName) const;

        bool Exists(const Name& withName) const;
    private:
        typedef ObjectBatch<RegistryObject> Batch;
    private:
        Batch batch;
    private:
        INSCRIPTION_ACCESS;
    };

    template<class T>
    RegistryObjectSystem<T>::RegistryObjectSystem(ObjectManager& manager) : ObjectSystem(manager)
    {
        batch = manager.Batch<RegistryObject>();
    }

    template<class T>
    RegistryObjectSystem<T>::RegistryObjectSystem(const ::Inscription::Table<RegistryObjectSystem>& table) :
        INSCRIPTION_TABLE_GET_BASE(ObjectSystem), INSCRIPTION_TABLE_GET_MEM(batch)
    {}

    template<class T>
    typename RegistryObjectSystem<T>::Reference RegistryObjectSystem<T>::FindRegistryObject(const Name& withName) const
    {
        for (auto& loop : batch)
            if (loop->name == withName)
                return loop.Downcast<T>();

        return Reference();
    }

    template<class T>
    bool RegistryObjectSystem<T>::Exists(const Name& withName) const
    {
        return FindAsset(withName).IsOccupied();
    }
}

namespace Inscription
{
    template<class T>
    class Inscripter<::Atmos::RegistryObjectSystem<T>> : public InscripterBase<::Atmos::RegistryObjectSystem<T>>
    {
    public:
        INSCRIPTION_INSCRIPTER_BASE_TYPEDEFS(::Atmos::RegistryObjectSystem<T>);
    public:
        INSCRIPTION_BINARY_INSCRIPTER_DECLARE_TABLE;
    };

    template<class T>
    typename Inscripter<::Atmos::RegistryObjectSystem<T>>::TableT Inscripter<::Atmos::RegistryObjectSystem<T>>::CreateTable(BinaryScribe& scribe)
    {
        INSCRIPTION_INSCRIPTER_CREATE_TABLE;

        INSCRIPTION_TABLE_ADD_BASE(::Atmos::ObjectSystem);
        INSCRIPTION_TABLE_ADD(batch);

        INSCRIPTION_INSCRIPTER_RETURN_TABLE;
    }
}