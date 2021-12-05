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
        static constexpr ObjectType objectType = ObjectType::Shard;
        static const inline TypeName typeName = "Atmos::Asset::Core";
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
        template<class Format>
        void Scriven(ObjectT& object, Format& format)
        {
            format("name", object.name);
        }
    };

    template<class Format>
    struct ScribeTraits<Atmos::Asset::Core, Format> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Asset::Core>;
    };
}