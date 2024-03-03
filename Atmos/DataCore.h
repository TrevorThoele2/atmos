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
        static const ObjectType objectType = ObjectType::Shard;
        static TypeName TypeName() { return "Atmos::DataCore"; }
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
        template<class Archive>
        void Scriven(ObjectT& object, Archive& archive)
        {
            archive("properties", object.properties);
        }
    };

    template<class Archive>
    struct ScribeTraits<Atmos::DataCore, Archive> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::DataCore>;
    };
}