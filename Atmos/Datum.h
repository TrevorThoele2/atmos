#pragma once

#include "Variant.h"

#include "Serialization.h"

namespace Atmos
{
    struct Datum
    {
        String name;
        Variant value;

        [[nodiscard]] bool operator==(const Datum& arg) const;
        [[nodiscard]] bool operator!=(const Datum& arg) const;
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Datum> final
    {
    public:
        using ObjectT = Atmos::Datum;
    public:
        template<class Archive>
        void Scriven(ObjectT& object, Archive& archive)
        {
            archive("name", object.name);
            archive("value", object.value);
        }
    };

    template<class Archive>
    struct ScribeTraits<Atmos::Datum, Archive> final
    {
        using Category = CompositeScribeCategory<Atmos::Datum>;
    };
}