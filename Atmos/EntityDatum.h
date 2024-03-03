#pragma once

#include "EntityVariant.h"

#include "Serialization.h"

namespace Atmos::Entity
{
    class Datum
    {
    public:
        String name;
        Variant value;
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Entity::Datum, BinaryArchive> final :
        public CompositeScribe<Atmos::Entity::Datum, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}