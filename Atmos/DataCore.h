#pragma once

#include "ArcaShardIncludes.h"

#include "Property.h"

#include <Arca/Serialization.h>
#include <Inscription/VectorScribe.h>

namespace Atmos
{
    struct DataCore
    {
        std::vector<Property> properties;

        DataCore() = default;
        DataCore(const std::vector<Property>& properties);

        bool operator==(const DataCore& arg) const;
        bool operator!=(const DataCore& arg) const;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::DataCore>
    {
        static constexpr ObjectType objectType = ObjectType::Shard;
        static const inline TypeName typeName = "Atmos::DataCore";
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::DataCore> final
    {
    public:
        using ObjectT = Atmos::DataCore;
    public:
        template<class Format>
        void Scriven(ObjectT& object, Format& format)
        {
            format("properties", object.properties);
        }
    };

    template<class Format>
    struct ScribeTraits<Atmos::DataCore, Format> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::DataCore>;
    };
}