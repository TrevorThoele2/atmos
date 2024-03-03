#pragma once

#include <Arca/Shard.h>

#include "Name.h"

namespace Atmos::Asset
{
    struct Core
    {
        Name name;

        Core() = default;
        explicit Core(const Name& name);
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Asset::Core>
    {
        static const ObjectType objectType = ObjectType::Shard;
        static inline const TypeName typeName = "AssetCore";
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Asset::Core, BinaryArchive> final :
        public ArcaCompositeScribe<Atmos::Asset::Core, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}