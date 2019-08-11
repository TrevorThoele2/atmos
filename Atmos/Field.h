#pragma once

#include <vector>

#include "FieldID.h"
#include "ObjectManager.h"

#include "ReadonlyProperty.h"

#include "Serialization.h"

namespace Atmos::World
{
    class Field
    {
    public:
        typedef FieldID ID;
        ReadonlyProperty<ID> id;
    public:
        ObjectManager objectManager;
    public:
        Field(ID id);
        Field(Field&& arg);
        Field& operator=(Field&& arg);
    private:
        ID _id;
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::World::Field, BinaryArchive> :
        public CompositeScribe<::Atmos::World::Field, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
        void ConstructImplementation(ObjectT* storage, ArchiveT& archive) override;
    };
}