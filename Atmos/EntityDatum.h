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
    class Scribe<Atmos::Entity::Datum> final
    {
    public:
        using ObjectT = Atmos::Entity::Datum;
    public:
        template<class Archive>
        void Scriven(ObjectT& object, Archive& archive)
        {
            archive("name", object.name);
            archive("value", object.value);
        }
    };

    template<class Archive>
    struct ScribeTraits<Atmos::Entity::Datum, Archive> final
    {
        using Category = CompositeScribeCategory<Atmos::Entity::Datum>;
    };
}