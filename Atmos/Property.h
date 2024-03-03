#pragma once

#include "Variant.h"
#include "String.h"

#include "Serialization.h"

namespace Atmos
{
    struct Property
    {
        String name;

        using Value = Variant;
        Value value;

        [[nodiscard]] bool operator==(const Property& arg) const;
        [[nodiscard]] bool operator!=(const Property& arg) const;
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Property> final
    {
    public:
        using ObjectT = Atmos::Property;
    public:
        template<class Archive>
        void Scriven(ObjectT& object, Archive& archive)
        {
            archive("name", object.name);
            archive("value", object.value);
        }
    };

    template<class Archive>
    struct ScribeTraits<Atmos::Property, Archive> final
    {
        using Category = CompositeScribeCategory<Atmos::Property>;
    };
}