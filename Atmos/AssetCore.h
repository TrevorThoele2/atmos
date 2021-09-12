#pragma once

#include "ArcaShardIncludes.h"

#include "Name.h"

namespace Atmos::Asset
{
    struct Core
    {
        Name name;

        Core() = default;
        explicit Core(const Name& name);

        bool operator==(const Core& arg) const;
        bool operator!=(const Core& arg) const;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Asset::Core>
    {
        static const ObjectType objectType = ObjectType::Shard;
        static TypeName TypeName() { return "Atmos::Asset::Core"; }
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Asset::Core> final
    {
    public:
        using ObjectT = Atmos::Asset::Core;
    public:
        template<class Archive>
        void Scriven(ObjectT& object, Archive& archive)
        {
            archive("name", object.name);
        }
    };

    template<class Archive>
    struct ScribeTraits<Atmos::Asset::Core, Archive> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Asset::Core>;
    };
}