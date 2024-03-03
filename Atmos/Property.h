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
        template<class Format>
        void Scriven(ObjectT& object, Format& format)
        {
            format("name", object.name);
            format("value", object.value);
        }
    };

    template<class Format>
    struct ScribeTraits<Atmos::Property, Format> final
    {
        using Category = CompositeScribeCategory<Atmos::Property>;
    };
}