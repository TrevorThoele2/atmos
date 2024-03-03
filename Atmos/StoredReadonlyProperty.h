#pragma once

#include "ReadonlyProperty.h"

#include "Serialization.h"

namespace Atmos
{
    template<class Stored, class Interface = Stored>
    class StoredReadonlyProperty : public ReadonlyProperty<Interface>
    {
    public:
        StoredReadonlyProperty();
        explicit StoredReadonlyProperty(Stored initialValue);
        StoredReadonlyProperty(const StoredReadonlyProperty& arg);
        StoredReadonlyProperty(StoredReadonlyProperty&& arg);
    private:
        Stored stored;
    private:
        INSCRIPTION_ACCESS;
    };

    template<class Stored, class Interface>
    StoredReadonlyProperty<Stored, Interface>::StoredReadonlyProperty() :
        ReadonlyProperty<Interface>([this]() { return stored; }), stored()
    {}

    template<class Stored, class Interface>
    StoredReadonlyProperty<Stored, Interface>::StoredReadonlyProperty(Stored initialValue) :
        ReadonlyProperty<Interface>([this]() { return stored; }), stored(initialValue)
    {}

    template<class Stored, class Interface>
    StoredReadonlyProperty<Stored, Interface>::StoredReadonlyProperty(const StoredReadonlyProperty& arg) :
        ReadonlyProperty<Interface>([this]() { return stored; }), stored(arg.stored)
    {}

    template<class Stored, class Interface>
    StoredReadonlyProperty<Stored, Interface>::StoredReadonlyProperty(StoredReadonlyProperty&& arg) :
        ReadonlyProperty<Interface>([this]() { return stored; }), stored(std::move(arg.stored))
    {}
}

namespace Inscription
{
    template<class Stored, class Interface>
    class Scribe<::Atmos::StoredReadonlyProperty<Stored, Interface>, BinaryArchive> :
        public CompositeScribe<::Atmos::StoredReadonlyProperty<Stored, Interface>, BinaryArchive>
    {
    private:
        using BaseT = typename CompositeScribe<::Atmos::StoredReadonlyProperty<Stored, Interface>, BinaryArchive>;
    public:
        using ObjectT = typename BaseT::ObjectT;
        using ArchiveT = typename BaseT::ArchiveT;

        using BaseT::Scriven;
        using BaseT::Construct;
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override
        {
            archive(object.stored);
        }

        void ConstructImplementation(ObjectT* storage, ArchiveT& archive) override
        {
            DoBasicConstruction(storage, archive);
        }

        using BaseT::DoBasicConstruction;
    };
}